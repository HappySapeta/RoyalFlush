// Copyright (c) 2026 VINNIE BRIGHTEY, FELICITY ZABAVA, ARTHUR NORTH, JOSH BENNETTS, LEWIS TAIT, KYLE MURRAY, HOLLY ALBERT, ANUPAM SAHU, ARAMINTA MCDIARMID. All rights reserved.

#include "RFPokerStates.h"
#include "Algo/RandomShuffle.h"
#include "Kismet/KismetMathLibrary.h"
#include "StateMachine/RpStateMachineBlackboard.h"

void URFPokerBeginState::OnActivate()
{
	const int ScoreMultiplier = Blackboard->GetValuesAsInt(ScoreMultiplierKey);
	int RoundNum = Blackboard->GetValuesAsInt(RoundNumKey);
	Blackboard->SetValuesAsInt(PoolMoneyKey, BASE_SCORE_MULTIPLIER * ScoreMultiplier);
	Blackboard->SetValuesAsInt(RoundNumKey, ++RoundNum);
	Blackboard->SetValuesAsInt(CurrentPlayerIndexKey, 0);
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
	Blackboard->GetValueChangeCallback(DiscardedHandKey).AddUniqueDynamic(this, &URFPokerDiscardingState::HandleDiscardRequested);
	
	SetTurn(EPokerPlayer::NPC);
	PlayNPCTurn();
	
	Blackboard->SetValuesAsBool(PassStatusKey, false);
	Blackboard->SetValuesAsBool(DiscardStatusKey, false);
	
	SetTurn(EPokerPlayer::Human);
	BP_OnHumanTurn();
	Super::OnActivate();
}

void URFPokerDiscardingState::SetTurn(EPokerPlayer Player)
{
	const FString PlayerName = Player == EPokerPlayer::Human ? TEXT("Human") : TEXT("NPC");
	UE_LOG(LogTemp, Warning, TEXT("Turn changed : %s"), *PlayerName);
	CurrentPlayer = Player;
	BP_OnTurnChanged(Player);
}

void URFPokerDiscardingState::NPCDiscard()
{
	// TODO: NPC Discard implementation.
}

void URFPokerDiscardingState::PlayNPCTurn()
{
	UE_LOG(LogTemp, Warning, TEXT("NPC choosing to discard or pass"));
	int RandomChoice = 1;//UKismetMathLibrary::RandomIntegerInRange(0,1);
	switch (RandomChoice)
	{
		case 0:
		{
			UE_LOG(LogTemp, Warning, TEXT("NPC chose to Discard"));
			NPCDiscard();
			BP_OnNPCDiscard();
			break;
		}
		case 1:
		{
			UE_LOG(LogTemp, Warning, TEXT("NPC chose to Pass"));
			BP_OnNPCPass();
			break;
		}
		default:
			break;
	}
	
	BP_OnNPCPlayed();
}

void URFPokerDiscardingState::HandleDiscardRequested(const FGameplayTag& Key)
{
	UObject* DiscardedHandObject = Blackboard->GetValuesAsObject(DiscardedHandKey);
	URFHand* DiscardedHand = Cast<URFHand>(DiscardedHandObject);
	URFCards* CardsObject = Cast<URFCards>(Blackboard->GetValuesAsObject(CardsKey));
	
	if (CurrentPlayer == EPokerPlayer::NPC)
	{
		URFHand* Hand = Cast<URFHand>(Blackboard->GetValuesAsObject(NPCHandKey));
		TArray<int> Cards = Hand->GetCards();
		
		CardsObject->ReplaceDiscardedCards(DiscardedHand->GetCards(), Cards);
		Hand->SetCards(Cards);
		
		Blackboard->SetValuesAsObject(NPCHandKey, Hand);
	}
	else if (CurrentPlayer == EPokerPlayer::Human)
	{
		URFHand* Hand = Cast<URFHand>(Blackboard->GetValuesAsObject(HumanHandKey));
		TArray<int> Cards = Hand->GetCards();
		
		CardsObject->ReplaceDiscardedCards(DiscardedHand->GetCards(), Cards);
		Hand->SetCards(Cards);
		
		int NumDiscards = Blackboard->GetValuesAsInt(DiscardNumKey);
		Blackboard->SetValuesAsInt(DiscardNumKey, NumDiscards + 1);
		Blackboard->SetValuesAsBool(DiscardStatusKey, true);
		Blackboard->SetValuesAsObject(HumanHandKey, Hand);
	}
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

URFCards::URFCards()
{
	for (int Index = 0; Index < NUM_PLAYING_CARDS; ++Index)
	{
		Cards.Push(Index);
	}
}

void URFCards::Shuffle()
{
	Algo::RandomShuffle(Cards);
}

TArray<int> URFCards::NewHand()
{
	int NextHandStartIndex = LastHandEndIndex + 1;
	if (!ensureAlways(Cards.IsValidIndex(NextHandStartIndex) && Cards.IsValidIndex(NextHandStartIndex + (HAND_SIZE - 1))))
	{
		return {};
	}
	
	int* Start = &Cards[NextHandStartIndex];
	LastHandEndIndex = NextHandStartIndex + (HAND_SIZE - 1);
	
	TArray<int>NewHand{Start, HAND_SIZE};
	
	for (int Index = NextHandStartIndex; Index <= LastHandEndIndex; ++Index)
	{
		Cards.RemoveAtSwap(Index, 1, EAllowShrinking::No);
	}
	
	Cards.Shrink();
	
	return NewHand;
}

void URFCards::Reset()
{
	LastHandEndIndex = -1;
	for (int Index = 0; Index < NUM_PLAYING_CARDS; ++Index)
	{
		Cards.Push(Index);
	}
}

int URFCards::SwapCard(int Card)
{
	int NewCard = Cards[0];
	Cards.RemoveAtSwap(0);
	Cards.Push(Card);
	return NewCard;
}

void URFCards::ReplaceDiscardedCards(const TArray<int> CardIndicesToBeDiscarded, TArray<int>& TargetHand)
{
	for (int CardIndex : CardIndicesToBeDiscarded)
	{
		if (CardIndex == -1)
		{
			continue;
		}
		
		TargetHand[CardIndex] = SwapCard(TargetHand[CardIndex]); 
	}
	
	Cards.Shrink();
}

TArray<int> URFHand::GetCards() const
{
	return Cards;
}

void URFHand::SetCards(const TArray<int>& NewCards)
{
	ensure(NewCards.Num() == HAND_SIZE);
	Cards = NewCards;
}

bool URFHand::Equals(const URFHand* Other) const
{
	for (int Index = 0; Index < HAND_SIZE; ++Index)
	{
		if (Cards[Index] != Other->Cards[Index])
		{
			return false;
		}
	}
	
	return true;
}

int URFRankedHands::GetRank(URFHand* Hand)
{
	int RankIndex = RankedHands.Num();
	for (int Index = 0; Index < RankedHands.Num(); ++Index)
	{
		if (Hand->Equals(RankedHands[Index]))
		{
			RankIndex = Index;
			break;
		}
	}
	
	return RankIndex;
}

bool URFRankedHands::IsFirstHigherThanSecond(URFHand* First, URFHand* Second)
{
	int FirstRank = GetRank(First);
	int SecondRank = GetRank(Second);
	return FirstRank < SecondRank;
}
