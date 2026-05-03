// Copyright (c) 2026 VINNIE BRIGHTEY, FELICITY ZABAVA, ARTHUR NORTH, JOSH BENNETTS, LEWIS TAIT, KYLE MURRAY, HOLLY ALBERT, ANUPAM SAHU, ARAMINTA MCDIARMID. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "RFGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class ROYALFLUSH_API URFGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable)
	bool GetGameResult() const
	{
		return bDidPlayerWin;
	}
	
	UFUNCTION(BlueprintCallable)
	void SetGameResult(const bool bWin)
	{
		bDidPlayerWin = bWin;
	}

private:
	
	UPROPERTY(VisibleAnywhere)
	bool bDidPlayerWin = false;
	
};
