// Copyright (c) 2026 VINNIE BRIGHTEY, FELICITY ZABAVA, ARTHUR NORTH, JOSH BENNETTS, LEWIS TAIT, KYLE MURRAY, HOLLY ALBERT, ANUPAM SAHU, ARAMINTA MCDIARMID. All rights reserved.

#include "RFJournalWidget.h"

#include "Kismet/GameplayStatics.h"
#include "RFJournalPageWidget.h"
#include "RoyalFlush/RFGameMode.h"

void URFJournalWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	FScriptDelegate Delegate;
	Delegate.BindUFunction(this, TEXT("HandleOnVisibilityChanged"));
	OnVisibilityChanged.AddUnique(Delegate);
}

void URFJournalWidget::HandleOnVisibilityChanged(ESlateVisibility InVisibility)
{
	if (InVisibility == ESlateVisibility::Visible)
	{
		PopulateClueWidgets();
	}
}

URFJournalPageWidget* URFJournalWidget::CreateNewPage()
{
	URFJournalPageWidget* NewPage = Cast<URFJournalPageWidget>(CreateWidget(this, JournalPageWidgetClass));
	ScrollBox->AddChild(NewPage);
	
	return NewPage;
}

void URFJournalWidget::PopulateClueWidgets()
{
	ARFGameMode* RFGameMode = Cast<ARFGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	
	if (!ScrollBox->HasAnyChildren())
	{
		LastJournalPage = CreateNewPage();
	}
		
	const TArray<FRFClue> NewClues = GetNewClues(RFGameMode->GetJournalComponent()->GetAllClues());
	CurrentClues.Append(NewClues);
	
	for (const FRFClue& Clue : NewClues)
	{
		if (!LastJournalPage->CanContainMoreClues())
		{
			LastJournalPage = CreateNewPage();
		}
		
		LastJournalPage->AddClue(Clue);
	}
}

TArray<FRFClue> URFJournalWidget::GetNewClues(const TArray<FRFClue>& Clues)
{
	TArray<FRFClue> NewClues;
	
	for (const FRFClue& Clue : Clues)
	{
		if (!CurrentClues.Contains(Clue))
		{
			NewClues.Push(Clue);
		}
	}
	
	return NewClues;
}
