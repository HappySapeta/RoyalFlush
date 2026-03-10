// Copyright (c) 2026 VINNIE BRIGHTEY, FELICITY ZABAVA, ARTHUR NORTH, JOSH BENNETTS, LEWIS TAIT, KYLE MURRAY, HOLLY ALBERT, ANUPAM SAHU, ARAMINTA MCDIARMID. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RFGameCameraManager.generated.h"

class UCameraComponent;

UCLASS()
class ROYALFLUSH_API ARFGameCameraManager : public AActor
{
	GENERATED_BODY()

public:

	// Sets default values for this actor's properties
	ARFGameCameraManager();
	
	virtual void Tick(float DeltaSeconds) override;
	
protected:
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UCameraComponent> CameraComponent;
};
