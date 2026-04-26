// Copyright (c) 2026 VINNIE BRIGHTEY, FELICITY ZABAVA, ARTHUR NORTH, JOSH BENNETTS, LEWIS TAIT, KYLE MURRAY, HOLLY ALBERT, ANUPAM SAHU, ARAMINTA MCDIARMID. All rights reserved.

#include "RFPokerStates.h"
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
	FTimerHandle TimerHandle;
	TimerManager.SetTimer(TimerHandle, Callback, Delay, false);
}

void URFPokerBeginState::OnActivate()
{
	const int ScoreMultiplier = Blackboard->GetValuesAsInt(ScoreMultiplierKey);
	Blackboard->SetValuesAsInt(PoolMoneyKey, BASE_SCORE_MULTIPLIER * ScoreMultiplier);
	Blackboard->SetValuesAsBool(RoundRestartKey, false);
	Blackboard->SetValuesAsObject(CardsKey, NewObject<URFCards>());
	Blackboard->SetValuesAsBool(GameEndStatusKey, false);
	
	Super::OnActivate();
}

void URFPokerDealingState::OnActivate()
{
	int RoundNum = Blackboard->GetValuesAsInt(RoundNumKey);
	Blackboard->SetValuesAsInt(RoundNumKey, ++RoundNum);
	
	GetBlackboard()->SetValuesAsInt(PotMoneyKey, 0);
	URFCards* CardsObject = Cast<URFCards>(Blackboard->GetValuesAsObject(CardsKey));
	CardsObject->Reset();
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
	
	Super::OnActivate();
}

void URFPokerDiscardingState::OnActivate()
{
	Super::OnActivate();

	// First action
	{
		FTimerDelegate NPCTurnCallback = FTimerDelegate::CreateLambda([this]()
		{
			Blackboard->SetValuesAsBool(PassStatusKey, false);
			Blackboard->SetValuesAsBool(DiscardStatusKey, false);
			SetTurn(EPokerPlayer::NPC);
			PlayNPCTurn();
		});
		ExecuteWithDelay(NPCTurnCallback, NPCTurnDelay);
	}
	
	// Second action
	{
		FTimerDelegate HumanTurnCallback = FTimerDelegate::CreateLambda([this]()
		{
			UE_LOG(LogTemp, Warning, TEXT("Human player's turn."));
			Blackboard->SetValuesAsBool(PassStatusKey, false);
			Blackboard->SetValuesAsBool(DiscardStatusKey, false);
			SetTurn(EPokerPlayer::Human);
			Blackboard->GetValueChangeCallback(DiscardedHandKey).AddUniqueDynamic(this, &URFPokerDiscardingState::HandleDiscardRequested);
		});
		ExecuteWithDelay(HumanTurnCallback, HumanTurnDelay);
	}
}

void URFPokerDiscardingState::OnDeactivate()
{
	Blackboard->GetValueChangeCallback(DiscardedHandKey).RemoveAll(this);
	SetTurn(EPokerPlayer::None);
	Super::OnDeactivate();
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
	// set blackboard values
	{
		int PoolMoney = Blackboard->GetValuesAsInt(PoolMoneyKey);
		int ScoreMultiplier = Blackboard->GetValuesAsInt(ScoreMultiplierKey);
		int TransferAmount = 2 * ScoreMultiplier;
		PoolMoney -= TransferAmount;
		Blackboard->SetValuesAsInt(PoolMoneyKey, PoolMoney);
	
		int PotMoney = Blackboard->GetValuesAsInt(PotMoneyKey);
		PotMoney += TransferAmount;
		Blackboard->SetValuesAsInt(PotMoneyKey, PotMoney);
	}
	
	Blackboard->GetValueChangeCallback(PassStatusKey).AddUniqueDynamic(this, &URFBettingState::OnPlayerPassed);
	Blackboard->GetValueChangeCallback(FoldStatusKey).AddUniqueDynamic(this, &URFBettingState::OnPlayerFolded);
	Blackboard->GetValueChangeCallback(DoubleDownStatusKey).AddUniqueDynamic(this, &URFBettingState::OnPlayerDoubleDowned);
	
	FTimerDelegate NPCTurnCallback = FTimerDelegate::CreateLambda([this]()
	{
		UE_LOG(LogTemp, Warning, TEXT("NPC's Turn"));
		Blackboard->SetValuesAsBool(PassStatusKey, false);
		Blackboard->SetValuesAsBool(FoldStatusKey, false);
		Blackboard->SetValuesAsBool(DoubleDownStatusKey, false);
		SetTurn(EPokerPlayer::NPC);
		PlayNPCTurn();
	});
	ExecuteWithDelay(NPCTurnCallback, NPCTurnDelay);
	
	FTimerDelegate HumanTurnCallback = FTimerDelegate::CreateLambda([this]()
	{
		UE_LOG(LogTemp, Warning, TEXT("Human Player's Turn"));
		Blackboard->SetValuesAsBool(PassStatusKey, false);
		Blackboard->SetValuesAsBool(FoldStatusKey, false);
		Blackboard->SetValuesAsBool(DoubleDownStatusKey, false);
		SetTurn(EPokerPlayer::Human);
	});
	ExecuteWithDelay(HumanTurnCallback, HumanTurnDelay);
	
	Super::OnActivate();
}

void URFBettingState::OnDeactivate()
{
	Blackboard->GetValueChangeCallback(PassStatusKey).RemoveAll(this);
	Blackboard->GetValueChangeCallback(FoldStatusKey).RemoveAll(this);
	Blackboard->GetValueChangeCallback(DoubleDownStatusKey).RemoveAll(this);
	SetTurn(EPokerPlayer::None);
	Super::OnDeactivate();
}

void URFBettingState::PlayNPCTurn()
{
	int RandomChoice = 0; //UKismetMathLibrary::RandomIntegerInRange(0,2);
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
	Blackboard->SetValuesAsInt(CurrentTurnKey, static_cast<int>(CurrentPlayer));
}

void URFBettingState::OnPlayerPassed(const FGameplayTag& Key)
{
	const bool bDidPlayerPass = Blackboard->GetValuesAsBool(Key);
	if (bDidPlayerPass)
	{
		if (CurrentPlayer == EPokerPlayer::NPC)
		{
			UE_LOG(LogTemp, Warning, TEXT("NPC Passed."));
		}
		else if (CurrentPlayer == EPokerPlayer::Human)
		{
			UE_LOG(LogTemp, Warning, TEXT("Human Passed."));
		}
	}
}

void URFBettingState::OnPlayerFolded(const FGameplayTag& Key)
{
	const bool bDidPlayerFold = Blackboard->GetValuesAsBool(Key);
	
	if (bDidPlayerFold)
	{
		if (CurrentPlayer == EPokerPlayer::NPC)
		{
			UE_LOG(LogTemp, Warning, TEXT("NPC folded."));
		}
		else if (CurrentPlayer == EPokerPlayer::Human)
		{
			UE_LOG(LogTemp, Warning, TEXT("Human folded."));
		}
		
		const FGameplayTag OtherPlayerMoneyKey = CurrentPlayer == EPokerPlayer::Human ? NPCMoneyKey : HumanMoneyKey;
		
		int OtherPlayerMoney = Blackboard->GetValuesAsInt(OtherPlayerMoneyKey);
		int PotMoney = Blackboard->GetValuesAsInt(PotMoneyKey);
		
		OtherPlayerMoney += PotMoney;
		PotMoney = 0;
		
		Blackboard->SetValuesAsInt(PotMoneyKey, PotMoney);
		Blackboard->SetValuesAsInt(OtherPlayerMoneyKey, OtherPlayerMoney);
		Blackboard->SetValuesAsBool(RoundEndKey, true);
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
			UE_LOG(LogTemp, Warning, TEXT("NPC double down."));
		}
		else if (CurrentPlayer == EPokerPlayer::Human)
		{
			UE_LOG(LogTemp, Warning, TEXT("Human double down."));
		}
	}
}

void URFRevealState::OnActivate()
{
	if (!bRulesInit)
	{
		Rules.Push(NewObject<URFRoyalFlushRule>(this)); 
		Rules.Push(NewObject<URFStraightFlushRule>(this));
		Rules.Push(NewObject<URFFourOfAKindRule>(this));
		Rules.Push(NewObject<URFFullHouseRule>(this));
		Rules.Push(NewObject<URFFlushRule>(this));
		Rules.Push(NewObject<URFStraightRule>(this));
		Rules.Push(NewObject<URFThreeOfAKindRule>(this));
		Rules.Push(NewObject<URFTwoPairRule>(this));
		Rules.Push(NewObject<URFOnePairRule>(this));
		Rules.Push(NewObject<URFHighCardRule>(this));
		
		bRulesInit = true;
	}
	
	URFHand* HumanPlayerCards = Cast<URFHand>(Blackboard->GetValuesAsObject(HumanPlayerHandKey));
	URFHand* NPCCards = Cast<URFHand>(Blackboard->GetValuesAsObject(NPCHandKey));
	
	auto MoveMoneyToPlayer = [this](const FGameplayTag& PlayerKey)
	{
		int PlayerMoney = Blackboard->GetValuesAsInt(PlayerKey);
		int PotMoney = Blackboard->GetValuesAsInt(PotMoneyKey);
		
		PlayerMoney += PotMoney;
		PotMoney = 0;
		
		Blackboard->SetValuesAsInt(PlayerKey, PlayerMoney);
		Blackboard->SetValuesAsInt(PotMoneyKey, PotMoney);
	};
	
	EPokerRankComparision Comparision = CompareFirstToSecond(HumanPlayerCards, NPCCards);

	switch (Comparision)
	{
		case EPokerRankComparision::HIGHER:
		{
			UE_LOG(LogTemp, Warning, TEXT("Human Won."))
			MoveMoneyToPlayer(HumanMoneyKey);
			break;
		}
		case EPokerRankComparision::LOWER:
		{
			UE_LOG(LogTemp, Warning, TEXT("NPC Won."))
			MoveMoneyToPlayer(NPCMoneyKey);
			break;
		}
		case EPokerRankComparision::SAME:
		{
			UE_LOG(LogTemp, Warning, TEXT("Draw."))
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
			break;
		}
		default:
		{
			UE_LOG(LogTemp, Warning, TEXT("Unhandled case."));
		}
	}
	
	Super::OnActivate();
}

EPokerRankComparision URFRevealState::CompareFirstToSecond(URFHand* First, URFHand* Second)
{
	for (const URFPokerHandRuleBase* Rule : Rules)
	{
		bool bFirstSatisfiesRule = Rule->Test(First->GetCards());
		bool bSecondSatisfiesRule = Rule->Test(Second->GetCards());
		
		if (bFirstSatisfiesRule && bSecondSatisfiesRule)
		{
			UE_LOG(LogTemp, Warning, TEXT("Both satisfied rule : %s"), *Rule->GetName());
			return static_cast<EPokerRankComparision>(Rule->Compare(First->GetCards(), Second->GetCards()));
		}
		if (bFirstSatisfiesRule)
		{
			UE_LOG(LogTemp, Warning, TEXT("First won by rule : %s"), *Rule->GetName());
			return EPokerRankComparision::HIGHER;
		}
		else if (bSecondSatisfiesRule)
		{
			UE_LOG(LogTemp, Warning, TEXT("Second won by rule : %s"), *Rule->GetName());
			return EPokerRankComparision::LOWER;
		}
	}
	
	return EPokerRankComparision::SAME;
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
		Blackboard->SetValuesAsBool(RoundRestartKey, true);
	}
	
	Super::OnActivate();
}
