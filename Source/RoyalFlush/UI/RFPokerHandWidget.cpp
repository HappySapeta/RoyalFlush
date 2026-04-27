// Copyright (c) 2026 VINNIE BRIGHTEY, FELICITY ZABAVA, ARTHUR NORTH, JOSH BENNETTS, LEWIS TAIT, KYLE MURRAY, HOLLY ALBERT, ANUPAM SAHU, ARAMINTA MCDIARMID. All rights reserved.

#include "RFPokerHandWidget.h"
#include "Components/Button.h"

void URFPokerHandWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	bIsSelected.Init(false, 5);
	
	Buttons.Add(FirstCardButton);
	Buttons.Add(SecondCardButton);
	Buttons.Add(ThirdCardButton);
	Buttons.Add(FourthCardButton);
	Buttons.Add(FifthCardButton);
	
	FirstCardButton->OnClicked.AddUniqueDynamic(this, &URFPokerHandWidget::OnFirstCardSelected);
	SecondCardButton->OnClicked.AddUniqueDynamic(this, &URFPokerHandWidget::OnSecondCardSelected);
	ThirdCardButton->OnClicked.AddUniqueDynamic(this, &URFPokerHandWidget::OnThirdCardSelected);
	FourthCardButton->OnClicked.AddUniqueDynamic(this, &URFPokerHandWidget::OnFourthCardSelected);
	FifthCardButton->OnClicked.AddUniqueDynamic(this, &URFPokerHandWidget::OnFifthCardSelected);
}

void URFPokerHandWidget::SetAllowSelection(const bool bValue)
{
	bSelectionAllowed = bValue;
}

void URFPokerHandWidget::UnSelectAll()
{
	for (bool& bValue : bIsSelected)
	{
		bValue = false;
	}
}

void URFPokerHandWidget::ToggleCardSelection(const int Index)
{
	if (!bSelectionAllowed)
	{
		return;
	}
	
	ensureAlways(bIsSelected.IsValidIndex(Index) && Buttons.IsValidIndex(Index));
	if (bIsSelected[Index])
	{
		bIsSelected[Index] = false;
		OnCardUnSelectedEvent.Broadcast(Index);
	}
	else
	{
		bIsSelected[Index] = true;
		OnCardSelectedEvent.Broadcast(Index);
	}
}

void URFPokerHandWidget::OnFirstCardSelected()
{
	ToggleCardSelection(0);
}

void URFPokerHandWidget::OnSecondCardSelected()
{
	ToggleCardSelection(1);
}

void URFPokerHandWidget::OnThirdCardSelected()
{
	ToggleCardSelection(2);
}

void URFPokerHandWidget::OnFourthCardSelected()
{
	ToggleCardSelection(3);
}

void URFPokerHandWidget::OnFifthCardSelected()
{
	ToggleCardSelection(4);
}
