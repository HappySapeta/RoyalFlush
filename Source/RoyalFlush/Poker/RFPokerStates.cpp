// Copyright (c) 2026 VINNIE BRIGHTEY, FELICITY ZABAVA, ARTHUR NORTH, JOSH BENNETTS, LEWIS TAIT, KYLE MURRAY, HOLLY ALBERT, ANUPAM SAHU, ARAMINTA MCDIARMID. All rights reserved.

#include "RFPokerStates.h"
#include "Kismet/KismetMathLibrary.h"
#include "StateMachine/RpStateMachineBlackboard.h"

constexpr int BASE_SCORE_MULTIPLIER = 19;

void URFPokerState::OnActivate()
{
	Super::OnActivate();
	Blackboard->SetValuesAsInt(CurrentStateKey, static_cast<int>(CurrentState));
	Blackboard->SetValuesAsObject(StatusObjectKey, NewObject<URFPokerStatus>());
	CurrentStatusObject = Cast<URFPokerStatus>(Blackboard->GetValuesAsObject(StatusObjectKey));
}

void URFPokerState::OnDeactivate()
{
	ClearTimers();
	Super::OnDeactivate();
}

void URFPokerState::ExecuteWithDelay(FTimerDelegate Callback, const float Delay)
{
	const AActor* OwningActor = Cast<AActor>(Blackboard->GetValuesAsObject(OwningActorKey));
	FTimerManager& TimerManager = OwningActor->GetWorld()->GetTimerManager();
	FTimerHandle TimerHandle;
	TimerManager.SetTimer(TimerHandle, Callback, Delay, false);
}

void URFPokerState::ClearTimers()
{
	const AActor* OwningActor = Cast<AActor>(Blackboard->GetValuesAsObject(OwningActorKey));
	FTimerManager& TimerManager = OwningActor->GetWorld()->GetTimerManager();
	TimerManager.ClearAllTimersForObject(this);
}

void URFPokerBeginState::OnActivate()
{
	Blackboard->SetValuesAsInt(PoolMoneyKey, BASE_SCORE_MULTIPLIER * Blackboard->GetValuesAsInt(ScoreMultiplierKey));
	Blackboard->SetValuesAsBool(GameEndStatusKey, false);
	Blackboard->SetValuesAsObject(CardsObjectKey, NewObject<URFCards>());
	
	Super::OnActivate();
	BP_OnActivate();
}

void URFPokerDealingState::OnActivate()
{
	Super::OnActivate();
	
	// Set status.
	{
		CurrentStatusObject->SetStatus(TEXT("..Dealing Cards..."));
	}
	
	// Initialize blackboard values.
	{
		Blackboard->SetValuesAsInt(RoundNumKey, Blackboard->GetValuesAsInt(RoundNumKey) + 1);
		Blackboard->SetValuesAsInt(PotMoneyKey, 0);
	}
	
	// Initialize cards.
	URFCards* CardsObject = Cast<URFCards>(Blackboard->GetValuesAsObject(CardsKey));
	CardsObject->Reset();
	CardsObject->Shuffle();
		
	// Initialize hands.
	{
		auto InitializeHand = [this](const FGameplayTag& HandKey) -> void
		{
			if (!Blackboard->GetValuesAsObject(HandKey))
			{
				Blackboard->SetValuesAsObject(HandKey, NewObject<URFHand>());
			}
		};
	
		InitializeHand(NPCHandKey);
		InitializeHand(HumanPlayerHandKey);
	}
	
	// Draw hands.
	{
		auto DrawHand = [this, CardsObject](const FGameplayTag& HandKey) -> void
		{
			URFHand* HandObject = Cast<URFHand>(Blackboard->GetValuesAsObject(HandKey));
			HandObject->SetCards(CardsObject->NewHand());
			Blackboard->SetValuesAsObject(HandKey, HandObject);
		};
		DrawHand(NPCHandKey);
		DrawHand(HumanPlayerHandKey);
	}
	
	// End this state with a delay.
	ExecuteWithDelay(FTimerDelegate::CreateLambda([this]()
	{
		EndState();
	}), DealingStateDelay);
}

void URFPokerDiscardingState::OnActivate()
{
	Super::OnActivate();
	
	CurrentStatusObject = Cast<URFPokerStatus>(Blackboard->GetValuesAsObject(StatusObjectKey));

	// First action
	{
		CurrentStatusObject->SetStatus(TEXT("NPC Choosing..."));
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
			Blackboard->SetValuesAsBool(PassStatusKey, false);
			Blackboard->SetValuesAsBool(DiscardStatusKey, false);
			Blackboard->SetValuesAsInt(DiscardNumKey, 0);
			SetTurn(EPokerPlayer::Human);
			Blackboard->GetValueChangeCallback(PassStatusKey).AddUniqueDynamic(this, &URFPokerDiscardingState::HandlePlayerPassed);
			Blackboard->GetValueChangeCallback(DiscardedHandKey).AddUniqueDynamic(this, &URFPokerDiscardingState::HandleDiscardRequested);
		});
		ExecuteWithDelay(HumanTurnCallback, HumanTurnDelay);
	}
}

void URFPokerDiscardingState::OnDeactivate()
{
	Blackboard->GetValueChangeCallback(DiscardedHandKey).RemoveAll(this);
	Blackboard->GetValueChangeCallback(PassStatusKey).RemoveAll(this);
	Blackboard->GetValueChangeCallback(DiscardStatusKey).RemoveAll(this);
	SetTurn(EPokerPlayer::None);
	Super::OnDeactivate();
}

void URFPokerDiscardingState::SetTurn(EPokerPlayer Player)
{
	CurrentTurn = Player;
	Blackboard->SetValuesAsInt(CurrentTurnKey, static_cast<int>(CurrentTurn));

	switch (Player)
	{
		case EPokerPlayer::NPC:
		{
			CurrentStatusObject->SetStatus(TEXT("NPC's turn."));
			break;
		}
		case EPokerPlayer::Human:
		{
			CurrentStatusObject->SetStatus(TEXT("Your turn."));
			break;
		}
		default:
		break;
	}
}

void URFPokerDiscardingState::PlayNPCTurn()
{
	int RandomChoice = UKismetMathLibrary::RandomIntegerInRange(0,1);
	switch (RandomChoice)
	{
		case 0:
		{
			CurrentStatusObject->SetStatus(TEXT("NPC chose to discard."));
			Blackboard->SetValuesAsBool(DiscardStatusKey, true);
			break;
		}
		case 1:
		{
			CurrentStatusObject->SetStatus(TEXT("NPC chose to pass."));
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
	TArray<int> ReplacedCards = Hand->GetCards();
	CardsObject->ReplaceDiscardedCards(DiscardedHand->GetCards(), ReplacedCards);
	Hand->SetCards(ReplacedCards);
		
	int NumDiscards = Blackboard->GetValuesAsInt(DiscardNumKey);
	Blackboard->SetValuesAsInt(DiscardNumKey, NumDiscards + 1);
	Blackboard->SetValuesAsBool(DiscardStatusKey, true);
	Blackboard->SetValuesAsObject(HumanPlayerHandKey, Hand);
	
	if (NumDiscards + 1 == 2)
	{
		CurrentStatusObject->SetStatus(TEXT("Turn Ended"));
		ExecuteWithDelay(FTimerDelegate::CreateLambda([this]()
		{
			EndState();
		}), EndStateDelay);
	}
}

void URFPokerDiscardingState::HandlePlayerPassed(const FGameplayTag& Key)
{
	const bool bPlayerPassed = Blackboard->GetValuesAsBool(Key);
	if (bPlayerPassed)
	{
		CurrentStatusObject->SetStatus(TEXT("Turn Ended"));
		ExecuteWithDelay(FTimerDelegate::CreateLambda([this]()
		{
			EndState();
		}), EndStateDelay);
	}
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
	
	CurrentStatusObject = Cast<URFPokerStatus>(Blackboard->GetValuesAsObject(StatusObjectKey));
	CurrentStatusObject->SetStatus(TEXT("Betting Phase"));
	
	Blackboard->GetValueChangeCallback(PassStatusKey).AddUniqueDynamic(this, &URFBettingState::OnPlayerPassed);
	Blackboard->GetValueChangeCallback(FoldStatusKey).AddUniqueDynamic(this, &URFBettingState::OnPlayerFolded);
	Blackboard->GetValueChangeCallback(DoubleDownStatusKey).AddUniqueDynamic(this, &URFBettingState::OnPlayerDoubleDowned);
	
	FTimerDelegate NPCTurnCallback = FTimerDelegate::CreateLambda([this]()
	{
		Blackboard->SetValuesAsBool(PassStatusKey, false);
		Blackboard->SetValuesAsBool(FoldStatusKey, false);
		Blackboard->SetValuesAsBool(DoubleDownStatusKey, false);
		SetTurn(EPokerPlayer::NPC);
	});
	ExecuteWithDelay(NPCTurnCallback, NPCTurnDelay);
	
	FTimerDelegate NPCPlayCallback = FTimerDelegate::CreateLambda([this]()
	{
		PlayNPCTurn();
	});
	ExecuteWithDelay(NPCPlayCallback, NPCPlayDelay);
	
	FTimerDelegate HumanTurnCallback = FTimerDelegate::CreateLambda([this]()
	{
		if (Blackboard->GetValuesAsBool(FoldStatusKey))
		{
			EndState();
			return;
		}
		
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
	Blackboard->SetValuesAsBool(PassStatusKey, false);
	Blackboard->SetValuesAsBool(FoldStatusKey, false);
	Blackboard->SetValuesAsBool(DoubleDownStatusKey, false);
	SetTurn(EPokerPlayer::None);
	Super::OnDeactivate();
}

void URFBettingState::PlayNPCTurn()
{
	int RandomChoice = UKismetMathLibrary::RandomIntegerInRange(0,2);
	
	if (bNPCAlwaysPass)
	{
		RandomChoice = 0;	
	}
	
	switch (RandomChoice)
	{
		case 0:
		{
			CurrentStatusObject->SetStatus(TEXT("NPC chose to Pass."));
			Blackboard->SetValuesAsBool(PassStatusKey, true);
			break;
		}
		case 1:
		{
			CurrentStatusObject->SetStatus(TEXT("NPC chose to Fold."));
			Blackboard->SetValuesAsBool(FoldStatusKey, true);
			break;
		}
		case 2:
		{
			CurrentStatusObject->SetStatus(TEXT("NPC chose to Double-Down."));
			Blackboard->SetValuesAsBool(DoubleDownStatusKey, true);
			break;
		}
		default:
		{
			CurrentStatusObject->SetStatus(TEXT("NPC failed to chose."));
			break;
		}
	}
}

void URFBettingState::SetTurn(EPokerPlayer Player)
{
	CurrentTurn = Player;
	Blackboard->SetValuesAsInt(CurrentTurnKey, static_cast<int>(CurrentTurn));

	switch (Player)
	{
		case EPokerPlayer::NPC:
		{
			CurrentStatusObject->SetStatus(TEXT("NPC betting..."));
			break;
		}
		case EPokerPlayer::Human:
		{
			CurrentStatusObject->SetStatus(TEXT("Your turn."));
			break;
		}
		default:
			break;
	}
}

void URFBettingState::OnPlayerPassed(const FGameplayTag& Key)
{
	const bool bDidPlayerPass = Blackboard->GetValuesAsBool(Key);
	if (bDidPlayerPass)
	{
		if (CurrentTurn == EPokerPlayer::Human)
		{
			CurrentStatusObject->SetStatus(TEXT("You chose to pass."));
			EndState();
		}
	}
}

void URFBettingState::OnPlayerFolded(const FGameplayTag& Key)
{
	const bool bDidPlayerFold = Blackboard->GetValuesAsBool(Key);
	
	if (bDidPlayerFold)
	{
		const FGameplayTag OtherPlayerMoneyKey = CurrentTurn == EPokerPlayer::Human ? NPCMoneyKey : HumanMoneyKey;
		
		int OtherPlayerMoney = Blackboard->GetValuesAsInt(OtherPlayerMoneyKey);
		int PotMoney = Blackboard->GetValuesAsInt(PotMoneyKey);
		
		OtherPlayerMoney += PotMoney;
		PotMoney = 0;
		
		Blackboard->SetValuesAsInt(PotMoneyKey, PotMoney);
		Blackboard->SetValuesAsInt(OtherPlayerMoneyKey, OtherPlayerMoney);
		
		if (CurrentTurn == EPokerPlayer::Human)
		{
			CurrentStatusObject->SetStatus(TEXT("You chose to fold."));
			EndState();
		}
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
		
		if (CurrentTurn == EPokerPlayer::Human)
		{
			CurrentStatusObject->SetStatus(TEXT("You chose to double down."));
			EndState();
		}
	}
}

void URFRevealState::OnActivate()
{
	CurrentStatusObject = Cast<URFPokerStatus>(Blackboard->GetValuesAsObject(StatusObjectKey));
	
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
	
	const TPair<EPokerRankComparision, FString>& Comparision = CompareFirstToSecond(HumanPlayerCards, NPCCards);
	
	ExecuteWithDelay(FTimerDelegate::CreateLambda([this, Comparision, MoveMoneyToPlayer]()
	{
		const FString RuleName = Comparision.Get<1>();
		switch (Comparision.Get<0>())
		{
			case EPokerRankComparision::HIGHER:
			{
				CurrentStatusObject->SetStatus(FString::Printf(TEXT("You won by rule : %s"), *RuleName));
				MoveMoneyToPlayer(HumanMoneyKey);
				break;
			}
			case EPokerRankComparision::LOWER:
			{
				CurrentStatusObject->SetStatus(FString::Printf(TEXT("NPC won by rule : %s"), *RuleName));
				MoveMoneyToPlayer(NPCMoneyKey);
				break;
			}
			case EPokerRankComparision::SAME:
			{
				CurrentStatusObject->SetStatus(FString::Printf(TEXT("DRAW : %s"), *RuleName));
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
				check(0);
			}
		}
	}), DeclarationDelay);
	
	ExecuteWithDelay(FTimerDelegate::CreateLambda([this]()
	{
		CurrentStatusObject->SetStatus(TEXT("Turn Ended"));
		EndState();
	}), EndStateDelay);
	
	Super::OnActivate();
}

TPair<EPokerRankComparision, FString> URFRevealState::CompareFirstToSecond(URFHand* First, URFHand* Second)
{
	for (const URFPokerHandRuleBase* Rule : Rules)
	{
		bool bFirstSatisfiesRule = Rule->Test(First->GetCards());
		bool bSecondSatisfiesRule = Rule->Test(Second->GetCards());
		
		if (bFirstSatisfiesRule && bSecondSatisfiesRule)
		{
			UE_LOG(LogTemp, Warning, TEXT("Both satisfied rule : %s"), *Rule->GetName());
			return {static_cast<EPokerRankComparision>(Rule->Compare(First->GetCards(), Second->GetCards())), Rule->GetRuleName()};
		}
		if (bFirstSatisfiesRule)
		{
			UE_LOG(LogTemp, Warning, TEXT("First won by rule : %s"), *Rule->GetName());
			return {EPokerRankComparision::HIGHER, Rule->GetRuleName()};
		}
		else if (bSecondSatisfiesRule)
		{
			UE_LOG(LogTemp, Warning, TEXT("Second won by rule : %s"), *Rule->GetName());
			return {EPokerRankComparision::LOWER, Rule->GetRuleName()};
		}
	}
	
	return {EPokerRankComparision::SAME, "No rule"};
}

void URFEndOfRoundState::OnActivate()
{
	CurrentStatusObject = Cast<URFPokerStatus>(Blackboard->GetValuesAsObject(StatusObjectKey));
	
	CurrentStatusObject->SetStatus(TEXT("Round End"));

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
	
	ExecuteWithDelay(FTimerDelegate::CreateLambda([this]()
	{
		EndState();
	}), EndStateDelay);
	
	Super::OnActivate();
}
