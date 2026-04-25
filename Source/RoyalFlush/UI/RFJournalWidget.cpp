// Copyright (c) 2026 VINNIE BRIGHTEY, FELICITY ZABAVA, ARTHUR NORTH, JOSH BENNETTS, LEWIS TAIT, KYLE MURRAY, HOLLY ALBERT, ANUPAM SAHU, ARAMINTA MCDIARMID. All rights reserved.

#include "RFJournalWidget.h"

#include "Kismet/GameplayStatics.h"
#include "RFJournalPageWidget.h"
#include "Components/Button.h"
#include "RoyalFlush/RFGameMode.h"

void URFJournalWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	FScriptDelegate Delegate;
	Delegate.BindUFunction(this, TEXT("HandleOnVisibilityChanged"));
	OnVisibilityChanged.AddUnique(Delegate);
	
	if (const ARFGameMode* GameMode = Cast<ARFGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		if (URFJournalComponent* JournalComponent = GameMode->GetJournalComponent())
		{
			JournalComponent->OnClueStagedEvent.AddUniqueDynamic(this, &URFJournalWidget::HandleClueStaged);
			JournalComponent->OnClueUnStagedEvent.AddUniqueDynamic(this, &URFJournalWidget::HandleClueUnStaged);
		}
	}
	
	AcceptButton->OnClicked.AddUniqueDynamic(this, &URFJournalWidget::HandleAcceptButtonClicked);
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
	PageScroller->AddChild(NewPage);
	
	return NewPage;
}

void URFJournalWidget::HandleClueStaged(FRFClue ClueData)
{
	for (UWidget* ChildWidget : PageScroller->GetAllChildren())
	{
		if (URFJournalPageWidget* Page = Cast<URFJournalPageWidget>(ChildWidget))
		{
			CurrentClues.Remove(ClueData);
			Page->RemoveClue(ClueData);
			
			URFJournalClueWidget* ClueWidget = Cast<URFJournalClueWidget>(CreateWidget(this, ClueWidgetClass));
			ClueWidget->SetData(ClueData);
			CluesContainer->AddChild(ClueWidget);
		}
	}
}

void URFJournalWidget::HandleClueUnStaged(FRFClue TargetClue)
{
	for (UWidget* Child : CluesContainer->GetAllChildren())
	{
		if (URFJournalClueWidget* ClueWidget = Cast<URFJournalClueWidget>(Child))
		{
			if (ClueWidget->GetData() == TargetClue)
			{
				CluesContainer->RemoveChild(ClueWidget);
				PopulateClueWidgets();
				return;
			}
		}
	}
}

void URFJournalWidget::PopulateClueWidgets()
{
	ARFGameMode* RFGameMode = Cast<ARFGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	
	if (!PageScroller->HasAnyChildren())
	{
		LastJournalPage = CreateNewPage();
	}
		
	const TArray<FRFClue> NewClues = GetNewClues(RFGameMode->GetJournalComponent()->GetUnstagedClues());
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

void URFJournalWidget::HandleAcceptButtonClicked()
{
	if (const ARFGameMode* GameMode = Cast<ARFGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		if (URFJournalComponent* JournalComponent = GameMode->GetJournalComponent())
		{
			JournalComponent->Accept();
		}
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
