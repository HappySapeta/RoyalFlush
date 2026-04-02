// Copyright (c) 2026 VINNIE BRIGHTEY, FELICITY ZABAVA, ARTHUR NORTH, JOSH BENNETTS, LEWIS TAIT, KYLE MURRAY, HOLLY ALBERT, ANUPAM SAHU, ARAMINTA MCDIARMID. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "StateMachine/RpState.h"
#include "RFPokerStates.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class ROYALFLUSH_API URFPokerState : public URpState
{
	GENERATED_BODY()
};

UCLASS(Blueprintable, BlueprintType)
class ROYALFLUSH_API URFPokerBeginState : public URFPokerState
{
	GENERATED_BODY()
	
public:
	
	virtual void OnActivate_Implementation() override;
	
private:
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag PoolMoneyKey;  
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag RoundNumKey;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag CurrentPlayerIndexKey;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag ScoreMultiplierKey;
};