// Copyright (c) 2026 VINNIE BRIGHTEY, FELICITY ZABAVA, ARTHUR NORTH, JOSH BENNETTS, LEWIS TAIT, KYLE MURRAY, HOLLY ALBERT, ANUPAM SAHU, ARAMINTA MCDIARMID. All rights reserved.

#include "RFPokerHandWidget.h"
#include "Components/Button.h"

void URFPokerHandWidget::NativeConstruct()
{
	Super::NativeConstruct();
	FirstCardButton->OnClicked.AddUniqueDynamic(this, &URFPokerHandWidget::OnFirstCardSelected);
	SecondCardButton->OnClicked.AddUniqueDynamic(this, &URFPokerHandWidget::OnSecondCardSelected);
	ThirdCardButton->OnClicked.AddUniqueDynamic(this, &URFPokerHandWidget::OnThirdCardSelected);
	FourthCardButton->OnClicked.AddUniqueDynamic(this, &URFPokerHandWidget::OnFourthCardSelected);
	FifthCardButton->OnClicked.AddUniqueDynamic(this, &URFPokerHandWidget::OnFifthCardSelected);
}

void URFPokerHandWidget::OnFirstCardSelected()
{
	OnCardSelectedEvent.Broadcast(0);
}

void URFPokerHandWidget::OnSecondCardSelected()
{
	OnCardSelectedEvent.Broadcast(1);
}

void URFPokerHandWidget::OnThirdCardSelected()
{
	OnCardSelectedEvent.Broadcast(2);
}

void URFPokerHandWidget::OnFourthCardSelected()
{
	OnCardSelectedEvent.Broadcast(3);
}

void URFPokerHandWidget::OnFifthCardSelected()
{
	OnCardSelectedEvent.Broadcast(4);
}
