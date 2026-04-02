// Copyright (c) 2026 VINNIE BRIGHTEY, FELICITY ZABAVA, ARTHUR NORTH, JOSH BENNETTS, LEWIS TAIT, KYLE MURRAY, HOLLY ALBERT, ANUPAM SAHU, ARAMINTA MCDIARMID. All rights reserved.

#include "RFPokerStates.h"
#include "StateMachine/RpStateMachineBlackboard.h"

constexpr int BASE_SCORE_MULTIPLIER = 19;

void URFPokerBeginState::OnActivate_Implementation()
{
	Super::OnActivate_Implementation();
	
	const int ScoreMultiplier = GetBlackboard()->GetValuesAsInt(ScoreMultiplierKey);
	GetBlackboard()->SetValuesAsInt(PoolMoneyKey, BASE_SCORE_MULTIPLIER * ScoreMultiplier);
	GetBlackboard()->SetValuesAsInt(RoundNumKey, 1);
	GetBlackboard()->SetValuesAsInt(CurrentPlayerIndexKey, 0);
	
	// BP:
	// Switch camera to 1st person perspective
	// Enable Poker UI 
}
