// Copyright (c) 2026 VINNIE BRIGHTEY, FELICITY ZABAVA, ARTHUR NORTH, JOSH BENNETTS, LEWIS TAIT, KYLE MURRAY, HOLLY ALBERT, ANUPAM SAHU, ARAMINTA MCDIARMID. All rights reserved.

#include "RFGameCameraManager.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ARFGameCameraManager::ARFGameCameraManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
}

void ARFGameCameraManager::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	FMinimalViewInfo ViewInfo;
	CameraComponent->GetCameraView(DeltaSeconds, ViewInfo);

	FMatrix ViewProjectionMatrix, ProjectionMatrix, ViewMatrix;
	UGameplayStatics::GetViewProjectionMatrix(ViewInfo, ViewMatrix, ProjectionMatrix, ViewProjectionMatrix);
	
	FConvexVolume Frustum;
	GetViewFrustumBounds(Frustum, ViewProjectionMatrix, true);
	
	if (const AActor* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	{
		const FString Message = Frustum.IntersectPoint(PlayerCharacter->GetActorLocation()) ? "True" : "False";
	}
	
	if (ViewInfo.PreviousViewTransform)
	{
		UKismetSystemLibrary::DrawDebugFrustum(GetWorld(), *ViewInfo.PreviousViewTransform);
	}
}
