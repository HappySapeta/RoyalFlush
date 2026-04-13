// Copyright (c) 2026 VINNIE BRIGHTEY, FELICITY ZABAVA, ARTHUR NORTH, JOSH BENNETTS, LEWIS TAIT, KYLE MURRAY, HOLLY ALBERT, ANUPAM SAHU, ARAMINTA MCDIARMID. All rights reserved.

#include "RFInteractionComponent.h"

#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

void URFInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
	
	const AActor* OwningActor = GetOwner();
	InteractionTrigger = Cast<USphereComponent>(OwningActor->FindComponentByTag(USphereComponent::StaticClass(), TriggerComponentTag.GetTagName()));
	InteractionWidget = Cast<UWidgetComponent>(OwningActor->FindComponentByTag(UWidgetComponent::StaticClass(), WidgetComponentTag.GetTagName()));
	
	if (InteractionTrigger)
	{
		InteractionTrigger->OnComponentBeginOverlap.AddUniqueDynamic(this, &URFInteractionComponent::OnTriggerEnter);
		InteractionTrigger->OnComponentEndOverlap.AddUniqueDynamic(this, &URFInteractionComponent::OnTriggerExited);
	}
	
	SetShowInteractionWidget(false);
}

void URFInteractionComponent::OnTriggerEnter
(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult
)
{
	const AActor* PlayerActor = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (PlayerActor == OtherActor)
	{
		if (!bIsInteractable)
		{
			return;
		}
		
		SetShowInteractionWidget(true);
		InteractionInstigator = OtherActor;
		
		OnInteractionStarted.Broadcast(InteractionInstigator);
	}
}

void URFInteractionComponent::OnTriggerExited(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AActor* PlayerActor = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (PlayerActor == OtherActor)
	{
		if (!bIsInteractable)
		{
			return;
		}
		
		SetShowInteractionWidget(false);
		OnInteractionStopped.Broadcast(PlayerActor);
	}
}

void URFInteractionComponent::SetShowInteractionWidget(const bool bNewVisibility)
{
	if (InteractionWidget)
	{
		InteractionWidget->SetVisibility(bNewVisibility);
	}
}
