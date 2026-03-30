// Copyright (c) 2026 VINNIE BRIGHTEY, FELICITY ZABAVA, ARTHUR NORTH, JOSH BENNETTS, LEWIS TAIT, KYLE MURRAY, HOLLY ALBERT, ANUPAM SAHU, ARAMINTA MCDIARMID. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "RFInteractionComponent.generated.h"

class USphereComponent;
class UWidgetComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ROYALFLUSH_API URFInteractionComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnTriggerEnter
	(
		UPrimitiveComponent* OverlappedComponent, 
		AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult
	);


	UFUNCTION()
	void OnTriggerExited
	(
		UPrimitiveComponent* OverlappedComponent, 
		AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);

	UFUNCTION(BlueprintCallable)
	void SetShowInteractionWidget(const bool bNewVisibility);
	
protected:

	UPROPERTY(EditAnywhere)
	FGameplayTag TriggerComponentTag;
	
	UPROPERTY(EditAnywhere)
	FGameplayTag WidgetComponentTag;
	
	UPROPERTY(BlueprintReadOnly);
	TObjectPtr<UWidgetComponent> InteractionWidget;
	
	UPROPERTY()
	TObjectPtr<USphereComponent> InteractionTrigger;
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AActor> InteractionInstigator;
};
