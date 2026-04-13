// Copyright (c) 2026 VINNIE BRIGHTEY, FELICITY ZABAVA, ARTHUR NORTH, JOSH BENNETTS, LEWIS TAIT, KYLE MURRAY, HOLLY ALBERT, ANUPAM SAHU, ARAMINTA MCDIARMID. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "DlgSystem/DlgDialogueParticipant.h"
#include "GameFramework/Actor.h"
#include "RFConvoActor.generated.h"

class UDlgDialogue;
class URFInteractionComponent;

UCLASS()
class UConvoParticipant : public UObject, public IDlgDialogueParticipant
{
	GENERATED_BODY()
friend class ARFConvoActor;
public:
	
	virtual FName GetParticipantName_Implementation() const override
	{
		return ParticipantName;
	}
	
private:
	
	FName ParticipantName;
	
};

UCLASS()
class ROYALFLUSH_API ARFConvoActor : public AActor
{
	GENERATED_BODY()
	
public:

	// Sets default values for this actor's properties
	ARFConvoActor();
	
	UFUNCTION(BlueprintCallable)
	void StartConversation();

protected:
	
	virtual void BeginPlay() override;
	
private:
	
	UFUNCTION()
	void BroadcastDialogueAndProgress();

public:
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnDialogueUpdated(const FText& DialogueText);

	UFUNCTION(BlueprintImplementableEvent)
	void OnDialogueEnded();
	
protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<URFInteractionComponent> InteractionComponent;

	
private:
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UDlgDialogue> DialogueAsset;
	
	UPROPERTY(EditAnywhere)
	float DialogueDelay = 2.0f;
	
	UPROPERTY(EditAnywhere)
	FName FirstParticipantName = TEXT("Participant1");
	
	UPROPERTY(EditAnywhere)
	FName SecondParticipantName = TEXT("Participant2");
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> BaseSceneComponent;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> FirstCharMesh;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> SecondCharMesh;
	
	UPROPERTY()
	UConvoParticipant* Participant1;
	
	UPROPERTY()
	UConvoParticipant* Participant2;
	
	UPROPERTY()
	UDlgContext* DialogueContext;
	
	FTimerHandle DialogueTimerHandle;
};
