// Copyright (c) 2026 VINNIE BRIGHTEY, FELICITY ZABAVA, ARTHUR NORTH, JOSH BENNETTS, LEWIS TAIT, KYLE MURRAY, HOLLY ALBERT, ANUPAM SAHU, ARAMINTA MCDIARMID. All rights reserved.

#include "RFPokerWidget.h"
#include "Components/Button.h"
#include "RoyalFlush/Poker/RFPokerStates.h"
#include "StateMachine/RpStateMachineBlackboard.h"

void URFPokerWidget::SetBlackboard(URpStateMachineBlackboardBase* NewBlackboard)
{
	Blackboard = NewBlackboard;
	OnBlackboardSet();
}

void URFPokerWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	DiscardedHand = NewObject<URFHand>(this);
	//BettingState_PassButton->OnClicked.AddUniqueDynamic(this, &URFPokerWidget::HandleBettingStatePassButton);
	//BettingState_FoldButton->OnClicked.AddUniqueDynamic(this, &URFPokerWidget::HandleBettingStateFoldButton);
	//BettingState_DoubleDownButton->OnClicked.AddUniqueDynamic(this, &URFPokerWidget::HandleBettingStateDoubleDownButton);
}

void URFPokerWidget::HandlePlayerPressedDiscard()
{
	URFHand* PlayerHand = Cast<URFHand>(Blackboard->GetValuesAsObject(HumanPlayerHandKey));
	TArray<int> PlayerCards = PlayerHand->GetCards();
	
	TArray<int> CardsToBeReplaced;
	for (int DiscardedIndex : DiscardedCardIndices)
	{
		CardsToBeReplaced.Push(DiscardedIndex);
	}
	while (CardsToBeReplaced.Num() < HAND_SIZE)
	{
		CardsToBeReplaced.Push(-1);
	}
	
	DiscardedHand->SetCards(CardsToBeReplaced);
	
	Blackboard->SetValuesAsObject(DiscardedHandKey, DiscardedHand);
	DiscardedCardIndices.Empty();
}
