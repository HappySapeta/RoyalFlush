// Copyright (c) 2026 VINNIE BRIGHTEY, FELICITY ZABAVA, ARTHUR NORTH, JOSH BENNETTS, LEWIS TAIT, KYLE MURRAY, HOLLY ALBERT, ANUPAM SAHU, ARAMINTA MCDIARMID. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/GameModeBase.h"
#include "Poker/RFPokerTypes.h"
#include "RFGameMode.generated.h"

class URpStateMachineComponent;
class URFJournalComponent;

/**
 * 
 */
UCLASS()
class ROYALFLUSH_API ARFGameMode : public AGameModeBase
{
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPokerGameEndedDelegate, EPokerPlayer, WinningPlayer, FName, ParticipantName);
	
	GENERATED_BODY()

public:
	
	ARFGameMode();
	
	UFUNCTION(BlueprintCallable)
	void StartPokerGame(const FName ParticipantName);
	
	URFJournalComponent* GetJournalComponent() const
	{
		return JournalComponent;
	}
	
protected:

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void StopPokerGame();

	UFUNCTION()
	void OnPokerGameEnded(const FGameplayTag& Key);
	
	virtual void BeginPlay() override;
	
private:
	
	UFUNCTION()
	void HandleCluesSubmitted();
	
public:
	
	UPROPERTY(BlueprintAssignable)
	FOnPokerGameEndedDelegate OnPokerGameEndedEvent;

protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	URpStateMachineComponent* PokerStateMachine;
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<URFJournalComponent> JournalComponent;

private:
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag WinningPlayingKey;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag OwningActorKey;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag PokerScoreTag;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag GameEndKey;
	
	FName PokerParticipant;
};
