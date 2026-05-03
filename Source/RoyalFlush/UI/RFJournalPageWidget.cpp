// Copyright (c) 2026 VINNIE BRIGHTEY, FELICITY ZABAVA, ARTHUR NORTH, JOSH BENNETTS, LEWIS TAIT, KYLE MURRAY, HOLLY ALBERT, ANUPAM SAHU, ARAMINTA MCDIARMID. All rights reserved.

#include "RFJournalPageWidget.h"

#include "RFJournalClueWidget.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "Kismet/KismetMathLibrary.h"

void URFJournalPageWidget::AddClue(const FRFClue& ClueItem)
{
	if (Clues.Contains(ClueItem))
	{
		return;
	}
	
	Clues.Add(ClueItem);
	
	URFJournalClueWidget* ClueWidget = Cast<URFJournalClueWidget>(CreateWidget(this, ClueWidgetClass));
	ClueWidget->SetData(ClueItem);
	
	int NewRow;
	int NewCol;
	const FString DescriptionString = ClueItem.Description.ToString();
	if (ClueLocations.Contains(DescriptionString))
	{
		NewRow = ClueLocations[DescriptionString].Get<0>();
		NewCol = ClueLocations[DescriptionString].Get<1>();
	}
	else
	{
		int NumChildren = UniformGridPanel->GetChildrenCount();
		NewRow = (NumChildren) / NumColumns;
		NewCol = (NumChildren) % NumColumns;
		ClueLocations.Add(ClueItem.Description.ToString(), {NewRow, NewCol});
	}
	
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

void URFJournalPageWidget::RemoveClue(const FRFClue& TargetClue)
{
	for (UWidget* Child : UniformGridPanel->GetAllChildren())
	{
		if (URFJournalClueWidget* ClueWidget = Cast<URFJournalClueWidget>(Child))
		{
			if (ClueWidget->GetData() == TargetClue)
			{
				UniformGridPanel->RemoveChild(ClueWidget);
				return;
			}
		}
	}
}

bool URFJournalPageWidget::ContainsClue(const FRFClue& ClueItem)
{
	return Clues.Contains(ClueItem);
}
