// Copyright (c) 2026 VINNIE BRIGHTEY, FELICITY ZABAVA, ARTHUR NORTH, JOSH BENNETTS, LEWIS TAIT, KYLE MURRAY, HOLLY ALBERT, ANUPAM SAHU, ARAMINTA MCDIARMID. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RFGameCameraManager.generated.h"

class UCameraComponent;

struct FCameraCandidate
{
	float CosineScore;
	float Distance;
	ACameraActor* CameraActor;
};

UCLASS()
class ROYALFLUSH_API ARFGameCameraManager : public AActor
{
	GENERATED_BODY()

public:
	
	ARFGameCameraManager();
	
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;
	
private:

	void CheckCameras(float DeltaSeconds);

	float GetCandidateScore(UCameraComponent* CameraComponent);
	
	void SwitchToCamera(ACameraActor* TargetCameraActor);

private:
	
	// Time to blend between cameras.
	UPROPERTY(EditAnywhere)
	float BlendTime;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<APlayerController> PlayerController;
	
	UPROPERTY(VisibleAnywhere)
	TArray<ACameraActor*> CameraActors;
	
	UPROPERTY(VisibleAnywhere)
	ACameraActor* CurrentCameraActor;
};
