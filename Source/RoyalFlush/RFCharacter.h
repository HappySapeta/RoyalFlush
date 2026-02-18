// Copyright (c) 2026 VINNIE BRIGHTEY, FELICITY ZABAVA, ARTHUR NORTH, JOSH BENNETTS, LEWIS TAIT, KYLE MURRAY, HOLLY ALBERT, ANUPAM SAHU, ARAMINTA MCDIARMID. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RFCharacter.generated.h"

class URFDialogueComponent;

UCLASS()
class ROYALFLUSH_API ARFCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	// Sets default values for this character's properties
	ARFCharacter();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnInteractionStarted(const ARFCharacter* Other);
};
