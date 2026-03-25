// Copyright (c) 2026 VINNIE BRIGHTEY, FELICITY ZABAVA, ARTHUR NORTH, JOSH BENNETTS, LEWIS TAIT, KYLE MURRAY, HOLLY ALBERT, ANUPAM SAHU, ARAMINTA MCDIARMID. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "RFCamera.generated.h"

class UBoxComponent;

UCLASS()
class ROYALFLUSH_API ARFCamera : public ACameraActor
{
	GENERATED_BODY()

public:

	// Sets default values for this actor's properties
	ARFCamera();
	
	bool IsActorInRoom(const AActor* Other);

private:
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> RoomCollider;
};
