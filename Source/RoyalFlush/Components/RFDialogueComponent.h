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
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDialogueStartedDelegate);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDialogueEndedDelegate);
	
public:
	
	UFUNCTION(BlueprintCallable)
	void StartInteraction(const UDlgContext* Context, const URFDialogueComponent* Other);
	
	UFUNCTION(BlueprintCallable)
	void EndInteraction(const UDlgContext* Context, const URFDialogueComponent* Other);
	
	UFUNCTION(BlueprintCallable)
	void SetDialogueContext(const UDlgContext* Context)
	{
		DialogueContext = Context;
	}
	
	virtual FText GetParticipantDisplayName_Implementation(FName ActiveSpeaker) const override
	{
		return ParticipantDisplayName;
	}
	
	virtual FName GetParticipantName_Implementation() const override
	{
		return ParticipantName;
	}

public:
	
	UPROPERTY(BlueprintAssignable)
	FOnDialogueStartedDelegate OnDialogueStartedEvent;
	
	UPROPERTY(BlueprintAssignable)
	FOnDialogueEndedDelegate OnDialogueEndedEvent;
	
protected:
	
	UPROPERTY(BlueprintReadOnly)
	const UDlgContext* DialogueContext;
	
	UPROPERTY(BlueprintReadOnly)
	const URFDialogueComponent* OtherParticipant;
	
	UPROPERTY(EditAnywhere)
	FName ParticipantName;
	
	UPROPERTY(EditAnywhere)
	FText ParticipantDisplayName;
};
