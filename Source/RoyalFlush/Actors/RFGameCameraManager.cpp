// Copyright (c) 2026 VINNIE BRIGHTEY, FELICITY ZABAVA, ARTHUR NORTH, JOSH BENNETTS, LEWIS TAIT, KYLE MURRAY, HOLLY ALBERT, ANUPAM SAHU, ARAMINTA MCDIARMID. All rights reserved.

#include "RFGameCameraManager.h"

#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"

ARFGameCameraManager::ARFGameCameraManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ARFGameCameraManager::BeginPlay()
{
	Super::BeginPlay();
	
	// Find the player's controller.
	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	
	// Find all cameras in the world.
	{
		TArray<AActor*> Actors;
		UGameplayStatics::GetAllActorsOfClass
		(
			GetWorld(),
			ACameraActor::StaticClass(), 
			Actors
		);
	
		for (AActor* Actor : Actors)
		{
			CameraActors.Push(Cast<ACameraActor>(Actor));
		}
	}
}

void ARFGameCameraManager::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	CheckCameras(DeltaSeconds);
}

void ARFGameCameraManager::CheckCameras(float DeltaSeconds)
{
	if (!PlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("No player controller found. Cameras will not update."));
		return;
	}
	
	ACameraActor* DesiredCameraActor = nullptr;
	float HighestCosineSoFar = TNumericLimits<float>::Min();
	for (ACameraActor* CameraActor : CameraActors)
	{
		const float CosineScore = GetCandidateScore(CameraActor->GetCameraComponent());
		if (CosineScore > HighestCosineSoFar)
		{
			HighestCosineSoFar = CosineScore;
			DesiredCameraActor = CameraActor;
		}
	}
	
	if (DesiredCameraActor && DesiredCameraActor != CurrentCameraActor)
	{
		SwitchToCamera(DesiredCameraActor);
	}
}

float ARFGameCameraManager::GetCandidateScore(UCameraComponent* CameraComponent)
{
	if (const AActor* Pawn = PlayerController->GetPawn())
	{
		const FVector PawnDirection = (Pawn->GetActorLocation() - CameraComponent->GetComponentLocation()).GetSafeNormal();
		const float Cosine = FVector::DotProduct(PawnDirection, CameraComponent->GetForwardVector());
		return Cosine;
	}
	
	return -1.0f;
}

void ARFGameCameraManager::SwitchToCamera(ACameraActor* TargetCameraActor)
{
	CurrentCameraActor = TargetCameraActor;
	PlayerController->SetViewTargetWithBlend(CurrentCameraActor, BlendTime);
}

//void ARFGameCameraManager::IsPlayerInFrustum()
//{
//	FMinimalViewInfo ViewInfo;
//	CameraComponent->GetCameraView(DeltaSeconds, ViewInfo);
//
//	FMatrix ViewProjectionMatrix, ProjectionMatrix, ViewMatrix;
//	UGameplayStatics::GetViewProjectionMatrix(ViewInfo, ViewMatrix, ProjectionMatrix, ViewProjectionMatrix);
//	
//	FConvexVolume Frustum;
//	GetViewFrustumBounds(Frustum, ViewProjectionMatrix, true);
//	
//	if (const AActor* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
//	{
//		const FString Message = Frustum.IntersectPoint(PlayerCharacter->GetActorLocation()) ? "True" : "False";
//	}
//	
//	if (ViewInfo.PreviousViewTransform)
//	{
//		UKismetSystemLibrary::DrawDebugFrustum(GetWorld(), *ViewInfo.PreviousViewTransform);
//	}
//}