// Copyright (c) 2026 VINNIE BRIGHTEY, FELICITY ZABAVA, ARTHUR NORTH, JOSH BENNETTS, LEWIS TAIT, KYLE MURRAY, HOLLY ALBERT, ANUPAM SAHU, ARAMINTA MCDIARMID. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RFGameCameraManager.generated.h"

class ARFCamera;
class UCameraComponent;

UCLASS()
class ROYALFLUSH_API ARFGameCameraManager : public AActor
{
	GENERATED_BODY()

public:
	
	ARFGameCameraManager();
	
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;
	
	UFUNCTION(BlueprintCallable)
	void SwitchToFPS();
	
private:

	void CheckCameras(float DeltaSeconds);

	float GetCosineScore(UCameraComponent* CameraComponent, const AActor* Pawn);
	
	void SwitchToCamera(ARFCamera* TargetCameraActor);
	

private:
	
	// Time to blend between cameras.
	UPROPERTY(EditAnywhere)
	float BlendTime;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<ACameraActor> PokerCameraActor;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<APlayerController> PlayerController;
	
	UPROPERTY(VisibleAnywhere)
	TArray<ARFCamera*> CameraActors;
	
	UPROPERTY(VisibleAnywhere)
	ACameraActor* CurrentCameraActor;
};
