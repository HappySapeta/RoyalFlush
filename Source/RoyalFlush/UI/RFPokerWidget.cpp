// Copyright (c) 2026 VINNIE BRIGHTEY, FELICITY ZABAVA, ARTHUR NORTH, JOSH BENNETTS, LEWIS TAIT, KYLE MURRAY, HOLLY ALBERT, ANUPAM SAHU, ARAMINTA MCDIARMID. All rights reserved.

#include "RFPokerWidget.h"
#include "RFPokerHandWidget.h"
#include "RoyalFlush/Poker/RFPokerDeck.h"
#include "RoyalFlush/Poker/RFPokerTypes.h"
#include "StateMachine/RpStateMachineBlackboard.h"

void URFPokerWidget::SetBlackboard(URpStateMachineBlackboardBase* NewBlackboard)
{
	Blackboard = NewBlackboard;
	
	// Blackboard events.
	{
		Blackboard->GetValueChangeCallback(HumanPlayerHandKey).AddUniqueDynamic(this, &URFPokerWidget::OnHandUpdate);
		Blackboard->GetValueChangeCallback(CurrentTurnKey).AddUniqueDynamic(this, &URFPokerWidget::OnTurnChanged);
		Blackboard->GetValueChangeCallback(CurrentStateKey).AddUniqueDynamic(this, &URFPokerWidget::OnStateChanged);
	}
	
	OnBlackboardSet();
}

void URFPokerWidget::NativeConstruct()
{
	Super::NativeConstruct();
	DiscardedHand = NewObject<URFHand>(this);
	
	PokerHandWidget->OnCardSelectedEvent.AddUniqueDynamic(this, &URFPokerWidget::HandlePlayerSelectedCard);
}

void URFPokerWidget::HandlePlayerSelectedCard(int Index)
{
	DiscardedCardIndices.Add(Index);
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
