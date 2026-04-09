// Copyright (c) 2026 VINNIE BRIGHTEY, FELICITY ZABAVA, ARTHUR NORTH, JOSH BENNETTS, LEWIS TAIT, KYLE MURRAY, HOLLY ALBERT, ANUPAM SAHU, ARAMINTA MCDIARMID. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RFGameMode.generated.h"

class URFJournalComponent;

/**
 * 
 */
UCLASS()
class ROYALFLUSH_API ARFGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	
	ARFGameMode();

protected:
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<URFJournalComponent> JournalComponent;
	
};
