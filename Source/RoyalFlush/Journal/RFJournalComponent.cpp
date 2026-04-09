// Copyright (c) 2026 VINNIE BRIGHTEY, FELICITY ZABAVA, ARTHUR NORTH, JOSH BENNETTS, LEWIS TAIT, KYLE MURRAY, HOLLY ALBERT, ANUPAM SAHU, ARAMINTA MCDIARMID. All rights reserved.
#include "RFJournalComponent.h"

URFJournalComponent::URFJournalComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

int URFJournalComponent::AddClue(const URFClueDataAsset* ClueObject)
{
	const FRFClue& ClueData = ClueObject->GetData();
	if (!Clues.Contains(ClueData))
	{
		Clues.Add(ClueData);
		return Clues.Num() - 1;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Attempted to add existing clue."));
	return -1;
}

void URFJournalComponent::StageClue(const int ClueIndex)
{
	if (Clues.IsValidIndex(ClueIndex))
	{
		StagedClues.Push(Clues[ClueIndex]);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Attempted to stage invalid clue index."));
	}
}

void URFJournalComponent::GetAllClues(TArray<FRFClue>& Out_Clues)
{
	Out_Clues = Clues;
}

void URFJournalComponent::GetStagedClues(TArray<FRFClue>& Out_StagedClues)
{
	Out_StagedClues = StagedClues;
}

void URFJournalComponent::Accept()
{
	OnCluesSubmittedEvent.Broadcast();
}

void URFJournalComponent::BindOnCluesSubmitted(FOnCluesSubmittedDelegate Callback)
{
	OnCluesSubmittedEvent = Callback;
}
