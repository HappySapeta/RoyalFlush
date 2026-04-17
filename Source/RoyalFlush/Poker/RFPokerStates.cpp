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

void URFBettingState::SetTurn(EPokerPlayer Player)
{
	CurrentPlayer = Player;
	BP_OnTurnChanged(Player);
}

void URFBettingState::OnActivate()
{
	Super::OnActivate();
	 
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
	Blackboard->OnValueChanged(PassStatusKey).AddUObject(this, &URFBettingState::OnPlayerPassed);
	Blackboard->OnValueChanged(FoldStatusKey).AddUObject(this, &URFBettingState::OnPlayerFolded);
	Blackboard->OnValueChanged(DoubleDownStatusKey).AddUObject(this, &URFBettingState::OnPlayerDoubleDowned);
}

void URFBettingState::EndTurn()
{
	if (CurrentPlayer == EPokerPlayer::Human) // Last turn
	{
		SetFinished(true);
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
		else if (CurrentPlayer == Human)
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
		else if (CurrentPlayer == Human)
		{
			BP_OnHumanFold();
		}
		
		const FGameplayTag OtherPlayerMoneyKey = CurrentPlayer == Human ? NPCMoneyKey : HumanMoneyKey;
		
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
