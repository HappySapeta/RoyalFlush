// Copyright (c) 2026 VINNIE BRIGHTEY, FELICITY ZABAVA, ARTHUR NORTH, JOSH BENNETTS, LEWIS TAIT, KYLE MURRAY, HOLLY ALBERT, ANUPAM SAHU, ARAMINTA MCDIARMID. All rights reserved.

#include "RFJournalClueWidget.h"
#include "Components/Image.h"

void URFJournalClueWidget::SetData(FRFClue NewClueData)
{
	ClueData = NewClueData;
	ClueDescriptionText->SetText(NewClueData.Description);
	ClueImage->SetBrushFromTexture(NewClueData.Image);
}

const FRFClue& URFJournalClueWidget::GetData()
{
	return ClueData;
}
