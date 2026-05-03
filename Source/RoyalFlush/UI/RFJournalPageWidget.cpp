// Copyright (c) 2026 VINNIE BRIGHTEY, FELICITY ZABAVA, ARTHUR NORTH, JOSH BENNETTS, LEWIS TAIT, KYLE MURRAY, HOLLY ALBERT, ANUPAM SAHU, ARAMINTA MCDIARMID. All rights reserved.

#include "RFJournalPageWidget.h"

#include "RFJournalClueWidget.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "Kismet/KismetMathLibrary.h"

void URFJournalPageWidget::NativeConstruct()
{
	Super::NativeConstruct();
	for (int Row = 0; Row < NumRows; ++Row)
	{
		for (int Col = 0; Col < NumColumns; ++Col)
		{
			AvailableSlots.Push({Row, Col});
		}
	}
}

void URFJournalPageWidget::AddClue(const FRFClue& ClueItem)
{
	if (Clues.Contains(ClueItem))
	{
		return;
	}
	
	Clues.Add(ClueItem);
	
	URFJournalClueWidget* ClueWidget = Cast<URFJournalClueWidget>(CreateWidget(this, ClueWidgetClass));
	ClueWidget->SetData(ClueItem);
	
	const FString DescriptionString = ClueItem.Description.ToString();
	
	TPair<int, int> SlotAddress = AvailableSlots.Last();
	AvailableSlots.Pop();
	
	UUniformGridSlot* GridSlot = UniformGridPanel->AddChildToUniformGrid(ClueWidget, SlotAddress.Get<0>(), SlotAddress.Get<1>());
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
	return !AvailableSlots.IsEmpty();
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
				Clues.Remove(TargetClue);
				
				if (UUniformGridSlot* GridSlot = Cast<UUniformGridSlot>(Child->Slot))
				{
					AvailableSlots.Push({GridSlot->GetRow(), GridSlot->GetColumn()});
				}
				
				return;
			}
		}
	}
}

bool URFJournalPageWidget::ContainsClue(const FRFClue& ClueItem)
{
	return Clues.Contains(ClueItem);
}
