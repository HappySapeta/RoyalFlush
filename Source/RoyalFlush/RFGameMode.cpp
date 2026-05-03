// Copyright (c) 2026 VINNIE BRIGHTEY, FELICITY ZABAVA, ARTHUR NORTH, JOSH BENNETTS, LEWIS TAIT, KYLE MURRAY, HOLLY ALBERT, ANUPAM SAHU, ARAMINTA MCDIARMID. All rights reserved.

#include "RFGameMode.h"

#include "RFGameInstance.h"
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

void ARFGameMode::OnPokerGameEnded(const FGameplayTag& Key)
{
	if (PokerStateMachine->GetBlackboard()->GetValuesAsBool(Key))
	{
		const EPokerPlayer WinningPlayer = static_cast<EPokerPlayer>(PokerStateMachine->GetBlackboard()->GetValuesAsInt(WinningPlayingKey));
		OnPokerGameEndedEvent.Broadcast(WinningPlayer, PokerParticipant);
		StopPokerGame();
	}
}

void ARFGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	PokerStateMachine->Initialize();
	PokerStateMachine->GetBlackboard()->GetValueChangeCallback(GameEndKey).AddUniqueDynamic(this, &ARFGameMode::OnPokerGameEnded);
	JournalComponent->OnCluesSubmittedEvent.AddUniqueDynamic(this, &ARFGameMode::HandleCluesSubmitted);
}

void ARFGameMode::HandleCluesSubmitted()
{
	const TArray<FRFClue>& StagedClues = JournalComponent->GetStagedClues();
	if (StagedClues.IsEmpty())
	{
		return;
	}
	
	URFGameInstance* GameInstance = Cast<URFGameInstance>(GetGameInstance());
	checkf(GameInstance, TEXT("Game Instance class has not been set "));
	
	for (const FRFClue& Clue : StagedClues)
	{
		if (!Clue.IsReal)
		{
			GameInstance->SetGameResult(false);
			OnGameEnded();
			return;
		}
	}
	
	GameInstance->SetGameResult(true);
	OnGameEnded();
}

void ARFGameMode::StartPokerGame(const FName ParticipantName)
{
	if (AActor* PlayerActor = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	{
		PokerStateMachine->GetBlackboard()->SetValuesAsObject(OwningActorKey, PlayerActor);
		PokerStateMachine->GetBlackboard()->SetValuesAsInt(PokerScoreTag, 0);
		PokerParticipant = ParticipantName;
		PokerStateMachine->Start();
	}
}

void ARFGameMode::Debug_InvokeEndings(const bool bWin)
{
#ifdef UE_BUILD_DEVELOPMENT
	URFGameInstance* GameInstance = Cast<URFGameInstance>(GetGameInstance());
	checkf(GameInstance, TEXT("Game Instance class has not been set "));
	
	GameInstance->SetGameResult(bWin);
	OnGameEnded();
#endif
}
