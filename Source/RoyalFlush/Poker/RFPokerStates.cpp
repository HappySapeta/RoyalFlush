// Copyright (c) 2026 VINNIE BRIGHTEY, FELICITY ZABAVA, ARTHUR NORTH, JOSH BENNETTS, LEWIS TAIT, KYLE MURRAY, HOLLY ALBERT, ANUPAM SAHU, ARAMINTA MCDIARMID. All rights reserved.

#include "RFPokerStates.h"
#include "RFPokerTypes.h"
#include "Kismet/KismetMathLibrary.h"
#include "StateMachine/RpStateMachineBlackboard.h"

constexpr int BASE_SCORE_MULTIPLIER = 19;

void URFPokerState::OnActivate()
{
	Super::OnActivate();
	Blackboard->SetValuesAsInt(CurrentStateKey, static_cast<int>(CurrentState));
}

void URFPokerState::ExecuteWithDelay(FTimerDelegate Callback, const float Delay)
{
	const AActor* OwningActor = Cast<AActor>(Blackboard->GetValuesAsObject(OwningActorKey));
	FTimerManager& TimerManager = OwningActor->GetWorld()->GetTimerManager();
	TimerManager.ClearTimer(DelayTimerHandle);
	TimerManager.SetTimer(DelayTimerHandle, Callback, Delay, false);
}

void URFPokerBeginState::OnActivate()
{
	const int ScoreMultiplier = Blackboard->GetValuesAsInt(ScoreMultiplierKey);
	int RoundNum = Blackboard->GetValuesAsInt(RoundNumKey);
	Blackboard->SetValuesAsInt(PoolMoneyKey, BASE_SCORE_MULTIPLIER * ScoreMultiplier);
	Blackboard->SetValuesAsInt(RoundNumKey, ++RoundNum);
	Blackboard->SetValuesAsBool(RoundRestartKey, false);
	Blackboard->SetValuesAsObject(CardsKey, NewObject<URFCards>());
	Blackboard->SetValuesAsBool(GameEndStatusKey, false);
	
	Super::OnActivate();
}

void URFPokerDealingState::OnActivate()
{
	GetBlackboard()->SetValuesAsInt(PotMoneyKey, 0);
	URFCards* CardsObject = Cast<URFCards>(Blackboard->GetValuesAsObject(CardsKey));
	if (ensure(CardsObject))
	{
		CardsObject->Shuffle();
		
		// Initialize hands
		UObject* HandObject = GetBlackboard()->GetValuesAsObject(HumanPlayerHandKey);
		{
			if (!HandObject)
			{
				GetBlackboard()->SetValuesAsObject(HumanPlayerHandKey, NewObject<URFHand>());
			}
		
			HandObject = GetBlackboard()->GetValuesAsObject(NPCHandKey);
			if (!HandObject)
			{
				GetBlackboard()->SetValuesAsObject(NPCHandKey, NewObject<URFHand>());
			}
		}
		
		// Player draw hand
		HandObject = Blackboard->GetValuesAsObject(HumanPlayerHandKey);
		if (URFHand* PlayerHand = Cast<URFHand>(HandObject))
		{
			PlayerHand->SetCards(CardsObject->NewHand());
			Blackboard->SetValuesAsObject(HumanPlayerHandKey, PlayerHand);
		}
		// NPC draw hand
		HandObject = Blackboard->GetValuesAsObject(NPCHandKey);
		if (URFHand* NPCHand = Cast<URFHand>(HandObject))
		{
			NPCHand->SetCards(CardsObject->NewHand());
			Blackboard->SetValuesAsObject(NPCHandKey, NPCHand);
		}
	}
	
	Super::OnActivate();
}

void URFPokerDiscardingState::OnActivate()
{
	Super::OnActivate();
	Blackboard->SetValuesAsBool(PassStatusKey, false);
	Blackboard->SetValuesAsBool(DiscardStatusKey, false);
	
	SetTurn(EPokerPlayer::NPC);
	ExecuteWithDelay(FTimerDelegate::CreateUObject(this, &URFPokerDiscardingState::PlayNPCTurn), 1.0f);
	
	FTimerDelegate HumanTurnCallback = FTimerDelegate::CreateLambda([this]()
	{
		Blackboard->GetValueChangeCallback(DiscardedHandKey).AddUniqueDynamic(this, &URFPokerDiscardingState::HandleDiscardRequested);
		SetTurn(EPokerPlayer::Human);
	});
	ExecuteWithDelay(HumanTurnCallback, 1.0f);
}

void URFPokerDiscardingState::SetTurn(EPokerPlayer Player)
{
	CurrentTurn = Player;
	Blackboard->SetValuesAsInt(CurrentTurnKey, static_cast<int>(CurrentTurn));
}

void URFPokerDiscardingState::PlayNPCTurn()
{
	UE_LOG(LogTemp, Warning, TEXT("NPC choosing to discard or pass"));
	int RandomChoice = UKismetMathLibrary::RandomIntegerInRange(0,1);
	switch (RandomChoice)
	{
		case 0:
		{
			UE_LOG(LogTemp, Warning, TEXT("NPC chose to Discard"));
			Blackboard->SetValuesAsBool(DiscardStatusKey, true);
			break;
		}
		case 1:
		{
			UE_LOG(LogTemp, Warning, TEXT("NPC chose to Pass"));
			Blackboard->SetValuesAsBool(PassStatusKey, true);
			break;
		}
		default:
			break;
	}
}

void URFPokerDiscardingState::HandleDiscardRequested(const FGameplayTag& Key)
{
	UObject* DiscardedHandObject = Blackboard->GetValuesAsObject(DiscardedHandKey);
	URFHand* DiscardedHand = Cast<URFHand>(DiscardedHandObject);
	URFCards* CardsObject = Cast<URFCards>(Blackboard->GetValuesAsObject(CardsKey));
	
	URFHand* Hand = Cast<URFHand>(Blackboard->GetValuesAsObject(HumanPlayerHandKey));
	TArray<int> Cards = Hand->GetCards();
		
	CardsObject->ReplaceDiscardedCards(DiscardedHand->GetCards(), Cards);
	Hand->SetCards(Cards);
		
	int NumDiscards = Blackboard->GetValuesAsInt(DiscardNumKey);
	Blackboard->SetValuesAsInt(DiscardNumKey, NumDiscards + 1);
	Blackboard->SetValuesAsBool(DiscardStatusKey, true);
	Blackboard->SetValuesAsObject(HumanPlayerHandKey, Hand);
}

void URFBettingState::OnActivate()
{
	int PoolMoney = Blackboard->GetValuesAsInt(PoolMoneyKey);
	int ScoreMultiplier = Blackboard->GetValuesAsInt(ScoreMultiplierKey);
	int TransferAmount = 2 * ScoreMultiplier;
	PoolMoney -= TransferAmount;
	Blackboard->SetValuesAsInt(PoolMoneyKey, PoolMoney);
	
	int PotMoney = Blackboard->GetValuesAsInt(PotMoneyKey);
	PotMoney += TransferAmount;
	Blackboard->SetValuesAsInt(PotMoneyKey, PotMoney);
	
	CurrentPlayer = EPokerPlayer::NPC;
	SetTurn(EPokerPlayer::NPC);
	Blackboard->GetValueChangeCallback(PassStatusKey).AddUniqueDynamic(this, &URFBettingState::OnPlayerPassed);
	Blackboard->GetValueChangeCallback(FoldStatusKey).AddUniqueDynamic(this, &URFBettingState::OnPlayerFolded);
	Blackboard->GetValueChangeCallback(DoubleDownStatusKey).AddUniqueDynamic(this, &URFBettingState::OnPlayerDoubleDowned);
	
	//PlayNPCTurn();
	
	Super::OnActivate();
}

void URFBettingState::PlayNPCTurn()
{
	int RandomChoice = UKismetMathLibrary::RandomIntegerInRange(0,2);
	switch (RandomChoice)
	{
		case 0:
		{
			Blackboard->SetValuesAsBool(PassStatusKey, true);
			break;
		}
		case 1:
		{
			Blackboard->SetValuesAsBool(FoldStatusKey, true);
			break;
		}
		case 2:
		{
			Blackboard->SetValuesAsBool(DoubleDownStatusKey, true);
			break;
		}
		default:
			break;
	}
}

void URFBettingState::SetTurn(EPokerPlayer Player)
{
	CurrentPlayer = Player;
	BP_OnTurnChanged(Player);
}

void URFBettingState::EndTurn()
{
	if (CurrentPlayer == EPokerPlayer::Human) // Last turn
	{
		EndState();
	}
	else
	{
		SetTurn(EPokerPlayer::Human);
		BP_OnHumanPlayerTurn();
	}
}

void URFBettingState::OnPlayerPassed(const FGameplayTag& Key)
{
	const bool bDidPlayerPass = Blackboard->GetValuesAsBool(Key);
	if (bDidPlayerPass)
	{
		if (CurrentPlayer == EPokerPlayer::NPC)
		{
			BP_OnNPCPass();
		}
		else if (CurrentPlayer == EPokerPlayer::Human)
		{
			BP_OnHumanPass();
		}
		
		EndTurn();
	}
}

void URFBettingState::OnPlayerFolded(const FGameplayTag& Key)
{
	const bool bDidPlayerFold = Blackboard->GetValuesAsBool(Key);
	
	if (bDidPlayerFold)
	{
		if (CurrentPlayer == EPokerPlayer::NPC)
		{
			BP_OnNPCFold();
		}
		else if (CurrentPlayer == EPokerPlayer::Human)
		{
			BP_OnHumanFold();
		}
		
		const FGameplayTag OtherPlayerMoneyKey = CurrentPlayer == EPokerPlayer::Human ? NPCMoneyKey : HumanMoneyKey;
		
		int OtherPlayerMoney = Blackboard->GetValuesAsInt(OtherPlayerMoneyKey);
		int PotMoney = Blackboard->GetValuesAsInt(PotMoneyKey);
		
		OtherPlayerMoney += PotMoney;
		PotMoney = 0;
		
		Blackboard->SetValuesAsInt(PotMoneyKey, PotMoney);
		Blackboard->SetValuesAsInt(OtherPlayerMoneyKey, OtherPlayerMoney);
		Blackboard->SetValuesAsBool(RoundEndKey, true);
		
		EndTurn();
	}
}

void URFBettingState::OnPlayerDoubleDowned(const FGameplayTag& Key)
{
	const bool bDidPlayerDoubleDown = Blackboard->GetValuesAsBool(Key);
	if (bDidPlayerDoubleDown)
	{
		int PoolMoney = Blackboard->GetValuesAsInt(PoolMoneyKey);
		int PotMoney = Blackboard->GetValuesAsInt(PotMoneyKey);
		
		if (PoolMoney >= PotMoney)
		{
			PoolMoney -= PotMoney;
			PotMoney += PotMoney;
		}
		else
		{
			const int Available = PotMoney - PoolMoney;
			PoolMoney -= Available;
			PotMoney += Available;
		}
		
		Blackboard->SetValuesAsInt(PoolMoneyKey, PoolMoney);
		Blackboard->SetValuesAsInt(PotMoneyKey, PotMoney);
		
		if (CurrentPlayer == EPokerPlayer::NPC)
		{
			BP_OnNPCPass();
		}
		else if (CurrentPlayer == EPokerPlayer::NPC)
		{
			BP_OnHumanPass();
		}
		
		EndTurn();
	}
}

void URFRevealState::OnActivate()
{
	URFHand* HumanPlayerCards = Cast<URFHand>(Blackboard->GetValuesAsObject(HumanPlayerHandKey));
	URFHand* NPCCards = Cast<URFHand>(Blackboard->GetValuesAsObject(NPCHandKey));
	URFRankedHands* RankedHands = Cast<URFRankedHands>(Blackboard->GetValuesAsObject(RankedHandsKey));
	
	auto MoveMoneyToPlayer = [this](const FGameplayTag& PlayerKey)
	{
		int PlayerMoney = Blackboard->GetValuesAsInt(PlayerKey);
		int PotMoney = Blackboard->GetValuesAsInt(PotMoneyKey);
		
		PlayerMoney += PotMoney;
		PotMoney = 0;
		
		Blackboard->SetValuesAsInt(PlayerKey, PlayerMoney);
		Blackboard->SetValuesAsInt(PotMoneyKey, PotMoney);
	};
	
	if (RankedHands->IsFirstHigherThanSecond(HumanPlayerCards, NPCCards))
	{
		MoveMoneyToPlayer(HumanMoneyKey);
	}
	else if (RankedHands->IsFirstHigherThanSecond(NPCCards, HumanPlayerCards))
	{
		MoveMoneyToPlayer(NPCMoneyKey);
	}
	else
	{
		// TODO: handle draws.
		int HumanPlayerMoney = Blackboard->GetValuesAsInt(HumanMoneyKey);
		int NPCMoney = Blackboard->GetValuesAsInt(NPCMoneyKey);
		int PotMoney = Blackboard->GetValuesAsInt(PotMoneyKey);
		
		HumanPlayerMoney += PotMoney / 2;
		NPCMoney += PotMoney / 2;
		PotMoney = 0;
		
		Blackboard->SetValuesAsInt(HumanMoneyKey, HumanPlayerMoney);
		Blackboard->SetValuesAsInt(NPCMoneyKey, NPCMoney);
		Blackboard->SetValuesAsInt(PotMoneyKey, PotMoney);
	}
	
	Super::OnActivate();
}

void URFEndOfRoundState::OnActivate()
{
	int HumanPlayerMoney = Blackboard->GetValuesAsInt(HumanMoneyKey);
	int NPCMoney = Blackboard->GetValuesAsInt(NPCMoneyKey);
	
	int PoolMoney = Blackboard->GetValuesAsInt(PoolMoneyKey);
	
	int HalfPoolMoney = FMath::RoundToInt(PoolMoney / 2.0f);
	if (HumanPlayerMoney >= HalfPoolMoney || NPCMoney >= HalfPoolMoney)
	{
		const EPokerPlayer WinningPlayer = HumanPlayerMoney > NPCMoney ? EPokerPlayer::Human : EPokerPlayer::NPC;
		Blackboard->SetValuesAsInt(WinningPlayerKey, static_cast<int>(WinningPlayer));
		Blackboard->SetValuesAsBool(GameEndStatusKey, true);
	}
	else
	{
		int RoundNum = Blackboard->GetValuesAsInt(RoundNumKey);
		if (RoundNum < 5)
		{
			Blackboard->SetValuesAsBool(RoundRestartKey, true);
		}
	}
	
	Super::OnActivate();
}
