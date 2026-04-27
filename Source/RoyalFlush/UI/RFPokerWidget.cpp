// Copyright (c) 2026 VINNIE BRIGHTEY, FELICITY ZABAVA, ARTHUR NORTH, JOSH BENNETTS, LEWIS TAIT, KYLE MURRAY, HOLLY ALBERT, ANUPAM SAHU, ARAMINTA MCDIARMID. All rights reserved.

#include "RFPokerWidget.h"
#include "RFPokerHandWidget.h"
#include "Components/WidgetSwitcher.h"
#include "RoyalFlush/Poker/RFPokerDeck.h"
#include "RoyalFlush/Poker/RFPokerTypes.h"
#include "StateMachine/RpStateMachineBlackboard.h"

void URFPokerWidget::NativeConstruct()
{
	Super::NativeConstruct();
	DiscardedHand = NewObject<URFHand>(this);
	
	PokerHandWidget->OnCardSelectedEvent.AddUniqueDynamic(this, &URFPokerWidget::HandlePlayerSelectedCard);
	PokerHandWidget->OnCardUnSelectedEvent.AddUniqueDynamic(this, &URFPokerWidget::HandlePlayerUnSelectedCard);
}

void URFPokerWidget::SetBlackboard(URpStateMachineBlackboardBase* NewBlackboard)
{
	Blackboard = NewBlackboard;
	
	// Blackboard events.
	{
		Blackboard->GetValueChangeCallback(HumanPlayerHandKey).AddUniqueDynamic(this, &URFPokerWidget::OnHandUpdate);
		Blackboard->GetValueChangeCallback(NPCHandKey).AddUniqueDynamic(this, &URFPokerWidget::OnOpponentHandUpdate);
		Blackboard->GetValueChangeCallback(CurrentTurnKey).AddUniqueDynamic(this, &URFPokerWidget::OnTurnChanged);
		Blackboard->GetValueChangeCallback(CurrentStateKey).AddUniqueDynamic(this, &URFPokerWidget::OnStateChanged);
		
		Blackboard->GetValueChangeCallback(PassStatusKey).AddUniqueDynamic(this, &URFPokerWidget::OnTurnPassed);
		Blackboard->GetValueChangeCallback(DoubleDownStatusKey).AddUniqueDynamic(this, &URFPokerWidget::OnDoubleDowned);
		Blackboard->GetValueChangeCallback(FoldStatusKey).AddUniqueDynamic(this, &URFPokerWidget::OnTurnFolded);
	}
	
	OnBlackboardSet();
}

void URFPokerWidget::HandlePlayerUnSelectedCard(int Index)
{
	DiscardedCardIndices.Remove(Index);
}

void URFPokerWidget::HandlePlayerSelectedCard(int Index)
{
	DiscardedCardIndices.Add(Index);
}

void URFPokerWidget::HandlePlayerPressedDiscard()
{
	if (DiscardedCardIndices.IsEmpty())
	{
		return;
	}
	
	int NumDiscarded = Blackboard->GetValuesAsInt(DiscardNumKey);
	if (NumDiscarded >= 1)
	{
		OverlaySwitcher->SetVisibility(ESlateVisibility::Hidden);	
	}
	
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
	PokerHandWidget->UnSelectAll();
}
