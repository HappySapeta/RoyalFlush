// Copyright (c) 2026 VINNIE BRIGHTEY, FELICITY ZABAVA, ARTHUR NORTH, JOSH BENNETTS, LEWIS TAIT, KYLE MURRAY, HOLLY ALBERT, ANUPAM SAHU, ARAMINTA MCDIARMID. All rights reserved.

#include "RFJournalPageWidget.h"

#include "RFJournalClueWidget.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "Kismet/KismetMathLibrary.h"

void URFJournalPageWidget::AddClue(const FRFClue& ClueItem)
{
	URFJournalClueWidget* ClueWidget = Cast<URFJournalClueWidget>(CreateWidget(this, ClueWidgetClass));
	ClueWidget->SetDescription(ClueItem.Description);
	ClueWidget->SetImage(ClueItem.Image);
	
	int NumChildren = UniformGridPanel->GetChildrenCount();
	
	int NewRow = (NumChildren) / NumColumns;
	int NewCol = (NumChildren) % NumColumns;
	
	UUniformGridSlot* GridSlot = UniformGridPanel->AddChildToUniformGrid(ClueWidget, NewRow, NewCol);
	GridSlot->GetContent()->SetRenderTranslation
	(
		{
			UKismetMathLibrary::RandomFloatInRange(-MaxRandomTranslation, MaxRandomTranslation), 
			UKismetMathLibrary::RandomFloatInRange(-MaxRandomTranslation, MaxRandomTranslation)
		}
	);
	
	GridSlot->GetContent()->SetRenderTransformAngle(UKismetMathLibrary::RandomFloatInRange(-MaxRandomAngle, MaxRandomAngle));
}

bool URFJournalPageWidget::CanContainMoreClues()
{
	return UniformGridPanel->GetChildrenCount() < ClueCapacity;
}
