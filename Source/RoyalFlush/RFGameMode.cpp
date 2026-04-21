// Copyright (c) 2026 VINNIE BRIGHTEY, FELICITY ZABAVA, ARTHUR NORTH, JOSH BENNETTS, LEWIS TAIT, KYLE MURRAY, HOLLY ALBERT, ANUPAM SAHU, ARAMINTA MCDIARMID. All rights reserved.

#include "RFGameMode.h"

#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Journal/RFJournalComponent.h"
#include "StateMachine/RpStateMachineBlackboard.h"
#include "StateMachine/RpStateMachineComponent.h"

ARFGameMode::ARFGameMode()
{
	PokerStateMachine = CreateDefaultSubobject<URpStateMachineComponent>(TEXT("PokerStateMachine"));
	JournalComponent = CreateDefaultSubobject<URFJournalComponent>(TEXT("JournalComponent"));
}

void ARFGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	PokerStateMachine->Initialize();
	JournalComponent->OnCluesSubmittedEvent.AddUniqueDynamic(this, &ARFGameMode::HandleCluesSubmitted);
}

void ARFGameMode::HandleCluesSubmitted()
{
	const TArray<FRFClue>& StagedClues = JournalComponent->GetStagedClues();
	if (StagedClues.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("Game lost!"));
		return;
	}
	
	for (const FRFClue& Clue : StagedClues)
	{
		if (!Clue.IsReal)
		{
			UE_LOG(LogTemp, Warning, TEXT("Game lost!"));
			return;
		}
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Game won!!"));
}

void ARFGameMode::StartPokerGame()
{
	if (AActor* PlayerActor = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	{
		PokerStateMachine->GetBlackboard()->SetValuesAsObject(PokerPlayerTag, PlayerActor);
		PokerStateMachine->GetBlackboard()->SetValuesAsInt(PokerScoreTag, 0);
		
		PokerStateMachine->Start();
	}
}
