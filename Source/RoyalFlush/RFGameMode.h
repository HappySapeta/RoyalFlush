// Copyright (c) 2026 VINNIE BRIGHTEY, FELICITY ZABAVA, ARTHUR NORTH, JOSH BENNETTS, LEWIS TAIT, KYLE MURRAY, HOLLY ALBERT, ANUPAM SAHU, ARAMINTA MCDIARMID. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/GameModeBase.h"
#include "RFGameMode.generated.h"

class URpStateMachineComponent;
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
	
	UFUNCTION(BlueprintCallable)
	void StartPokerGame();
	
	URFJournalComponent* GetJournalComponent()
	{
		return JournalComponent;
	}
	
protected:
	
	virtual void BeginPlay() override;
	
private:
	
	UPROPERTY(EditDefaultsOnly)
	URpStateMachineComponent* PokerStateMachine;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag PokerPlayerTag;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag PokerScoreTag;
	
protected:
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<URFJournalComponent> JournalComponent;
	
};
