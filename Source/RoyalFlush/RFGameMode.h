// Copyright (c) 2026 VINNIE BRIGHTEY, FELICITY ZABAVA, ARTHUR NORTH, JOSH BENNETTS, LEWIS TAIT, KYLE MURRAY, HOLLY ALBERT, ANUPAM SAHU, ARAMINTA MCDIARMID. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/GameModeBase.h"
#include "RFGameMode.generated.h"

UENUM(BlueprintType)
enum EPokerPlayer : uint8
{
	NPC,
	Player
};

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
	
	UFUNCTION(BlueprintCallable)
	void StartPokerGame();
	
	UFUNCTION(BlueprintCallable)
	void Poker_Pass(const EPokerPlayer Player);
	
	UFUNCTION(BlueprintCallable)
	void Poker_Discard(const EPokerPlayer Player);
	
protected:
	
	virtual void BeginPlay() override;

private:
	
	UPROPERTY(EditDefaultsOnly)
	URpStateMachineComponent* PokerStateMachine;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag PokerPlayerTag;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag PokerScoreTag;
};
