// Copyright (c) 2026 VINNIE BRIGHTEY, FELICITY ZABAVA, ARTHUR NORTH, JOSH BENNETTS, LEWIS TAIT, KYLE MURRAY, HOLLY ALBERT, ANUPAM SAHU, ARAMINTA MCDIARMID. All rights reserved.

#include "RFCamera.h"
#include "Components/BoxComponent.h"

// Sets default values
ARFCamera::ARFCamera()
{
	RoomCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("RoomCollider"));
}

bool ARFCamera::IsActorInRoom(const AActor* Other)
{
	return RoomCollider->IsOverlappingActor(Other);
}
