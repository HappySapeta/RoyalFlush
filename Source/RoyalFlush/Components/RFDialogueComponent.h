// Copyright (c) 2026 VINNIE BRIGHTEY, FELICITY ZABAVA, ARTHUR NORTH, JOSH BENNETTS, LEWIS TAIT, KYLE MURRAY, HOLLY ALBERT, ANUPAM SAHU, ARAMINTA MCDIARMID. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DlgSystem/DlgDialogueParticipant.h"
#include "RFDialogueComponent.generated.h"


UCLASS(DisplayName = "DialogueComponent", ClassGroup=(RoyalFlush), Blueprintable, meta=(BlueprintSpawnableComponent) )
class ROYALFLUSH_API URFDialogueComponent : public UActorComponent, public IDlgDialogueParticipant
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URFDialogueComponent();
 
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
