// Copyright (c) 2026 VINNIE BRIGHTEY, FELICITY ZABAVA, ARTHUR NORTH, JOSH BENNETTS, LEWIS TAIT, KYLE MURRAY, HOLLY ALBERT, ANUPAM SAHU, ARAMINTA MCDIARMID. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RFGameMode.generated.h"

class URpStateMachineComponent;
/**
 * 
 */
UCLASS()
class ROYALFLUSH_API ARFGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	
	ARFGameMode();

private:
	
	UPROPERTY(EditAnywhere)
	URpStateMachineComponent* PokerStateMachine;
};
