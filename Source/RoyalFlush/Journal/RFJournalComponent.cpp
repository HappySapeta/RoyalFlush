// Copyright (c) 2026 VINNIE BRIGHTEY, FELICITY ZABAVA, ARTHUR NORTH, JOSH BENNETTS, LEWIS TAIT, KYLE MURRAY, HOLLY ALBERT, ANUPAM SAHU, ARAMINTA MCDIARMID. All rights reserved.
#include "RFJournalComponent.h"

URFJournalComponent::URFJournalComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

int URFJournalComponent::GetNumCluesCollected() const
{
	return Clues.Num();
}

void URFJournalComponent::AddClue(FRFClue ClueData)
{
	if (!Clues.Contains(ClueData))
	{
		Clues.Add(ClueData);
		return;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Attempted to add existing clue."));
}

void URFJournalComponent::StageClue(FRFClue ClueData)
{
	const int Index = Clues.Find(ClueData);
	if (Clues.IsValidIndex(Index))
	{
		Clues[Index].IsStaged = true;
		OnClueStagedEvent.Broadcast(Clues[Index]);
	}
}

void URFJournalComponent::UnStageClue(FRFClue ClueData)
{
	const int Index = Clues.Find(ClueData);
	if (Clues.IsValidIndex(Index))
	{
		Clues[Index].IsStaged = false;
		OnClueUnStagedEvent.Broadcast(Clues[Index]);
	}
}

TArray<FRFClue> URFJournalComponent::GetUnstagedClues() const
{
	TArray<FRFClue> UnStagedClues;
	for (const FRFClue& Clue : Clues)
	{
		if (!Clue.IsStaged)
		{
			UnStagedClues.Push(Clue);
		}
	}
	
	return UnStagedClues;
}

TArray<FRFClue> URFJournalComponent::GetStagedClues()
{
	TArray<FRFClue> StagedClues;
	for (const FRFClue& Clue : Clues)
	{
		if (Clue.IsStaged)
		{
			StagedClues.Push(Clue);
		}
	}
	
	return StagedClues;
}

void URFJournalComponent::Accept()
{
	OnCluesSubmittedEvent.Broadcast();
}
