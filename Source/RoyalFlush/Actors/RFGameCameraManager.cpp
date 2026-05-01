// Copyright (c) 2026 VINNIE BRIGHTEY, FELICITY ZABAVA, ARTHUR NORTH, JOSH BENNETTS, LEWIS TAIT, KYLE MURRAY, HOLLY ALBERT, ANUPAM SAHU, ARAMINTA MCDIARMID. All rights reserved.

#include "RFGameCameraManager.h"

#include "RFCamera.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"

struct FCameraCandidate
{
	float CosineScore;
	ARFCamera* CameraActor;
};

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
			ARFCamera::StaticClass(), 
			Actors
		);
	
		for (AActor* Actor : Actors)
		{
			CameraActors.Push(Cast<ARFCamera>(Actor));
		}
	}
}

void ARFGameCameraManager::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	CheckCameras();
}

void ARFGameCameraManager::SwitchToPokerCamera(const AActor* Target, const float Duration)
{
	bIsInPokerMode = true;
	PlayerController->SetViewTargetWithBlend(PokerCameraActor, Duration);
}

void ARFGameCameraManager::SwitchToTopDown(float Duration)
{
	bIsInPokerMode = false;
	CheckCameras();
}

void ARFGameCameraManager::SwitchToDefaultCamera()
{
	CurrentCameraActor = PlayerController->GetPawn();
	PlayerController->SetViewTarget(CurrentCameraActor);
}

void ARFGameCameraManager::CheckCameras()
{
	if (bIsInPokerMode)
	{
		return;
	}
	
	if (!PlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("No player controller found. Cameras will not update."));
		return;
	}
	const AActor* PlayerPawn = PlayerController->GetPawn();
	
	static TArray<FCameraCandidate> CameraCandidates;
	CameraCandidates.Reserve(CameraActors.Num());
	
	for (ARFCamera* CameraActor : CameraActors)
	{
		if (CameraActor->IsActorInRoom(PlayerPawn))
		{
			const float CosineScore = GetCosineScore(CameraActor->GetCameraComponent(), PlayerPawn);
			CameraCandidates.Push(FCameraCandidate{CosineScore, CameraActor});
		}
	}
	
	ARFCamera* DesiredCameraTarget = nullptr;
	if (!CameraCandidates.IsEmpty())
	{
		CameraCandidates.Sort([](const FCameraCandidate& A, const FCameraCandidate& B)
		{
			return A.CosineScore > B.CosineScore;
		});
			
		DesiredCameraTarget = CameraCandidates[0].CameraActor;
		CameraCandidates.Empty();
	}
	
	if (DesiredCameraTarget && DesiredCameraTarget != CurrentCameraActor)
	{
		SwitchToCamera(DesiredCameraTarget);
	}
	else if (!DesiredCameraTarget)
	{
		SwitchToDefaultCamera();
	}
}

float ARFGameCameraManager::GetCosineScore(UCameraComponent* CameraComponent, const AActor* Pawn)
{
	const FVector PawnDirection = (Pawn->GetActorLocation() - CameraComponent->GetComponentLocation()).GetSafeNormal();
	const float Cosine = FVector::DotProduct(PawnDirection, CameraComponent->GetForwardVector());
	return Cosine;
}

void ARFGameCameraManager::SwitchToCamera(ARFCamera* TargetCameraActor)
{
	CurrentCameraActor = TargetCameraActor;
	PlayerController->SetViewTargetWithBlend(CurrentCameraActor, BlendTime);
}