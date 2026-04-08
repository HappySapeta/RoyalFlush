// Copyright (c) 2026 VINNIE BRIGHTEY, FELICITY ZABAVA, ARTHUR NORTH, JOSH BENNETTS, LEWIS TAIT, KYLE MURRAY, HOLLY ALBERT, ANUPAM SAHU, ARAMINTA MCDIARMID. All rights reserved.

#include "RFPokerStates.h"
#include "Algo/RandomShuffle.h"
#include "StateMachine/RpStateMachineBlackboard.h"

constexpr int BASE_SCORE_MULTIPLIER = 19;
constexpr int NUM_PLAYING_CARDS = 52;
constexpr int HAND_SIZE = 5;

void URFPokerBeginState::OnActivate()
{
	Super::OnActivate();
	SetFinished(false);
	
	const int ScoreMultiplier = GetBlackboard()->GetValuesAsInt(ScoreMultiplierKey);
	GetBlackboard()->SetValuesAsInt(PoolMoneyKey, BASE_SCORE_MULTIPLIER * ScoreMultiplier);
	GetBlackboard()->SetValuesAsInt(RoundNumKey, 1);
	GetBlackboard()->SetValuesAsInt(CurrentPlayerIndexKey, 0);
	GetBlackboard()->SetValuesAsObject(CardsKey, NewObject<URFCards>());
	
	BP_SwitchToPokerCamera();
	BP_EnablePokerUI();
}

void URFPokerDealingState::OnActivate()
{
	Super::OnActivate();
	SetFinished(false);
	
	GetBlackboard()->SetValuesAsInt(PotMoneyKey, 0);
	URFCards* CardsObject = Cast<URFCards>(GetBlackboard()->GetValuesAsObject(CardsKey));
	if (ensure(CardsObject))
	{
		CardsObject->Shuffle();
		
		// Initialize hands
		UObject* HandObject = GetBlackboard()->GetValuesAsObject(PlayerHandKey);
		{
			if (!HandObject)
			{
				GetBlackboard()->SetValuesAsObject(PlayerHandKey, NewObject<URFHand>());
			}
		
			HandObject = GetBlackboard()->GetValuesAsObject(NPCHandKey);
			if (!HandObject)
			{
				GetBlackboard()->SetValuesAsObject(NPCHandKey, NewObject<URFHand>());
			}
		}
		
		// Player draw hand
		HandObject = GetBlackboard()->GetValuesAsObject(PlayerHandKey);
		if (URFHand* PlayerHand = Cast<URFHand>(HandObject))
		{
			PlayerHand->SetHand(CardsObject->NewHand());
		}
		
		// NPC draw hand
		HandObject = GetBlackboard()->GetValuesAsObject(NPCHandKey);
		if (URFHand* NPCHand = Cast<URFHand>(HandObject))
		{
			NPCHand->SetHand(CardsObject->NewHand());
		}
	}
	
	OnUpdateUI();
}

void URFPokerDiscardingState::OnActivate()
{
	Super::OnActivate();
	SetFinished(false);
	BP_ShowPassDiscardUI(); 
}

void URFBettingState::OnActivate()
{
	Super::OnActivate();
	
	int PoolMoney = GetBlackboard()->GetValuesAsInt(PoolMoneyKey);
	int ScoreMultiplier = GetBlackboard()->GetValuesAsInt(ScoreMultiplierKey);
	int TransferAmount = 2 * ScoreMultiplier;
	PoolMoney -= TransferAmount;
	GetBlackboard()->SetValuesAsInt(PoolMoneyKey, PoolMoney);
	
	int PotMoney = GetBlackboard()->GetValuesAsInt(PotMoneyKey);
	PotMoney += TransferAmount;
	GetBlackboard()->SetValuesAsInt(PotMoneyKey, PotMoney);
}

void URFBettingState::StateUpdate_Implementation(const float DeltaTime)
{
	Super::StateUpdate_Implementation(DeltaTime);
	
	auto PlaySubprocess = [this](const EPokerPlayer CurrentPlayer)
	{
		bool bDidPlayerPass = GetBlackboard()->GetValuesAsBool(PassStatusKey);
		bool bDidPlayerFold = GetBlackboard()->GetValuesAsBool(FoldStatusKey);
		bool bDidPlayerDoubleDown = GetBlackboard()->GetValuesAsBool(DoubleDownStatusKey);
		
		if (bDidPlayerPass)
		{
			if (CurrentPlayer == EPokerPlayer::Player)
			{
				BP_OnPlayerPass();
			}
			else if (CurrentPlayer == EPokerPlayer::NPC)
			{
				BP_OnNPCPass();
			}
		}
		else if (bDidPlayerFold)
		{
			GetBlackboard()->SetValuesAsBool(RoundEndKey, true);
			const FGameplayTag CurrentPlayerMoneyKey = CurrentPlayer == Player ? PlayerMoneyKey : NPCMoneyKey;
		}
		else if (bDidPlayerDoubleDown)
		{
			int PoolMoney = GetBlackboard()->GetValuesAsInt(PoolMoneyKey);
			int PotMoney = GetBlackboard()->GetValuesAsInt(PotMoneyKey);
		
			PoolMoney -= PotMoney;
			PotMoney += PotMoney;
		
			GetBlackboard()->SetValuesAsInt(PoolMoneyKey, PoolMoney);
			GetBlackboard()->SetValuesAsInt(PotMoneyKey, PotMoney);
		
			if (CurrentPlayer == EPokerPlayer::Player)
			{
				BP_OnPlayerPass();
			}
			else if (CurrentPlayer == EPokerPlayer::NPC)
			{
				BP_OnNPCPass();
			}
		}
	};
	
	if (CurrentPlayer == EPokerPlayer::NPC)
	{
		static bool bHasNPCTurnUpdated = false;
		if (!bHasNPCTurnUpdated)
		{
			GetBlackboard()->SetValuesAsInt(CurrentTurnKey, EPokerPlayer::NPC);
			bHasNPCTurnUpdated = true;
		}
		PlaySubprocess(EPokerPlayer::NPC);
	}
	
	if (CurrentPlayer == EPokerPlayer::Player)
	{
		static bool bHasNPCTurnUpdated = false;
		if (!bHasNPCTurnUpdated)
		{
			GetBlackboard()->SetValuesAsInt(CurrentTurnKey, EPokerPlayer::NPC);
			bHasNPCTurnUpdated = true;
		}
		PlaySubprocess(EPokerPlayer::NPC);
	}
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
	if (!ensure(Cards.IsValidIndex(NextHandStartIndex) && Cards.IsValidIndex(NextHandStartIndex + (HAND_SIZE - 1))))
	{
		return {};
	}
	
	int* Start = &Cards[NextHandStartIndex];
	LastHandEndIndex = NextHandStartIndex + (HAND_SIZE - 1);
	return TArray<int>{Start, HAND_SIZE};
}

void URFCards::Reset()
{
	LastHandEndIndex = -1;
}

void URFHand::SetHand(const TArray<int>& NewHand)
{
	ensure(NewHand.Num() == HAND_SIZE);
	Cards = NewHand;
}
