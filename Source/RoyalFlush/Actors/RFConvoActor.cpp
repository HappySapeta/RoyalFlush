// Copyright (c) 2026 VINNIE BRIGHTEY, FELICITY ZABAVA, ARTHUR NORTH, JOSH BENNETTS, LEWIS TAIT, KYLE MURRAY, HOLLY ALBERT, ANUPAM SAHU, ARAMINTA MCDIARMID. All rights reserved.

#include "RFConvoActor.h"

#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "DlgSystem/DlgContext.h"
#include "DlgSystem/DlgManager.h"
#include "RoyalFlush/Components/RFInteractionComponent.h"

// Sets default values
ARFConvoActor::ARFConvoActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	BaseSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(BaseSceneComponent);
	
	FirstCharMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstSkeletalMeshComponent"));
	FirstCharMesh->SetupAttachment(GetRootComponent());
	
	FirstWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("FirstWidgetComponent"));
	FirstWidgetComponent->SetupAttachment(FirstCharMesh);
	
	SecondCharMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SecondSkeletalMeshComponent"));
	SecondCharMesh->SetupAttachment(GetRootComponent());
	
	SecondWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("SecondWidgetComponent"));
	SecondWidgetComponent->SetupAttachment(SecondCharMesh);
	
	InteractionComponent = CreateDefaultSubobject<URFInteractionComponent>(TEXT("RFInteraction"));
	InteractionComponent->SetupAttachment(GetRootComponent());
	
	FScriptDelegate Delegate;
	Delegate.BindUFunction(this, TEXT("StopConversation"));
	InteractionComponent->OnInteractionStopped.AddUnique(Delegate);
}

void ARFConvoActor::BeginPlay()
{
	Super::BeginPlay();
	Participant1 = NewObject<UConvoParticipant>();
	Participant1->ParticipantName = FirstParticipantName;
	
	Participant2 = NewObject<UConvoParticipant>();
	Participant2->ParticipantName = SecondParticipantName;
}

void ARFConvoActor::StartConversation()
{
	check(DialogueAsset);
	
	InteractionComponent->GetTriggerComponent()->SetSphereRadius(AudibleDistance);
	InteractionComponent->SetInteractable(false);
	
	DialogueContext = UDlgManager::StartDialogue2(DialogueAsset, Participant1, Participant2);
	if (DialogueContext->HasDialogueEnded())
	{
		UE_LOG(LogTemp, Warning, TEXT("Dialogue graph has no dialogues."));
		return;
	}
	
	BroadcastDialogueAndProgress();
	
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUObject(this, &ARFConvoActor::BroadcastDialogueAndProgress);
	GetWorld()->GetTimerManager().SetTimer(DialogueTimerHandle, TimerDelegate, DialogueDelay, true);
}

void ARFConvoActor::BroadcastDialogueAndProgress()
{
	if (DialogueContext->HasDialogueEnded())
	{
		OnDialogueEnded();
		return;
	}
	
	OnDialogueUpdated(DialogueContext->GetActiveNodeText(), DialogueContext->GetActiveNodeParticipantName());
	DialogueContext->ChooseOption(0);
}

void ARFConvoActor::StopConversation()
{
	GetWorld()->GetTimerManager().ClearTimer(DialogueTimerHandle);
	FirstWidgetComponent->SetVisibility(false);
	SecondWidgetComponent->SetVisibility(false);
}