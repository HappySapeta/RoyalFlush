// Copyright (c) 2026 VINNIE BRIGHTEY, FELICITY ZABAVA, ARTHUR NORTH, JOSH BENNETTS, LEWIS TAIT, KYLE MURRAY, HOLLY ALBERT, ANUPAM SAHU, ARAMINTA MCDIARMID. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "StateMachine/RpState.h"
#include "RFPokerRules.h"
#include "RFPokerTypes.h"
#include "RFPokerStates.generated.h"

class URFHand;
class URFCards;
class URFRankedHands;

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class ROYALFLUSH_API URFPokerState : public URpState
{
	GENERATED_BODY()
	
protected:
	
	virtual void OnActivate() override;
	
	void ExecuteWithDelay(FTimerDelegate Callback, const float Delay);
	
protected:
	
	UPROPERTY(EditDefaultsOnly, Category = "Blackboard")
	FGameplayTag OwningActorKey;
	
	UPROPERTY(EditDefaultsOnly, Category = "Blackboard")
	FGameplayTag CurrentStateKey;
	
	UPROPERTY(EditDefaultsOnly, Category = "Default")
	EPokerState CurrentState;
};

UCLASS(Blueprintable, BlueprintType)
class ROYALFLUSH_API URFPokerBeginState : public URFPokerState
{
	GENERATED_BODY()
	
protected:

	virtual void OnActivate() override;
	
private:
	
	UPROPERTY(EditDefaultsOnly, Category = "Blackboard")
	FGameplayTag PoolMoneyKey;
	
	UPROPERTY(EditDefaultsOnly, Category = "Blackboard")
	FGameplayTag RoundNumKey;
	
	UPROPERTY(EditDefaultsOnly, Category = "Blackboard")
	FGameplayTag ScoreMultiplierKey;
	
	UPROPERTY(EditDefaultsOnly, Category = "Blackboard")
	FGameplayTag CardsKey;
	
	UPROPERTY(EditDefaultsOnly, Category = "Blackboard")
	FGameplayTag RoundRestartKey;
	
	UPROPERTY(EditDefaultsOnly, Category = "Blackboard")
	FGameplayTag GameEndStatusKey;
	
	UPROPERTY(EditDefaultsOnly, Category = "Blackboard")
	FGameplayTag StatusObjectKey;
	
private:
	
	UPROPERTY()
	URFPokerStatus* StatusObject;
};

UCLASS(Blueprintable, BlueprintType)
class ROYALFLUSH_API URFPokerDealingState : public URFPokerState
{
	GENERATED_BODY()
	
protected:

	virtual void OnActivate() override;

private:
	
	UPROPERTY(EditDefaultsOnly, Category = "Default")
	float DealingStateDelay;

	UPROPERTY(EditDefaultsOnly, Category = "Blackboard")
	FGameplayTag PotMoneyKey;
	
	UPROPERTY(EditDefaultsOnly, Category= "Blackboard" )
	FGameplayTag CardsKey;
	
	UPROPERTY(EditDefaultsOnly, Category= "Blackboard" )
	FGameplayTag HumanPlayerHandKey;
	
	UPROPERTY(EditDefaultsOnly, Category= "Blackboard" )
	FGameplayTag NPCHandKey;
	
	UPROPERTY(EditDefaultsOnly, Category= "Blackboard" )
	FGameplayTag RoundNumKey;
};

UCLASS(Blueprintable, BlueprintType)
class ROYALFLUSH_API URFPokerDiscardingState : public URFPokerState
{
	GENERATED_BODY()
	
protected:

	virtual void OnActivate() override;
	
	virtual void OnDeactivate() override;

private:

	void SetTurn(EPokerPlayer Player);

	void PlayNPCTurn();
	
	UFUNCTION()
	void HandleDiscardRequested(const FGameplayTag& Key);
	
	UFUNCTION()
	void HandlePlayerPassed(const FGameplayTag& Key);
	
private:

	UPROPERTY(EditDefaultsOnly, Category = "Default")
	float NPCTurnDelay;
	
	UPROPERTY(EditDefaultsOnly, Category = "Default")
	float HumanTurnDelay;
	
	UPROPERTY(EditDefaultsOnly, Category = "Default")
	float EndStateDelay;
	
	UPROPERTY(EditDefaultsOnly, Category = "Blackboard")
	FGameplayTag PassStatusKey;
	
	UPROPERTY(EditDefaultsOnly, Category = "Blackboard")
	FGameplayTag DiscardStatusKey;
	
	UPROPERTY(EditDefaultsOnly, Category = "Blackboard")
	FGameplayTag DiscardedHandKey;
	
	UPROPERTY(EditDefaultsOnly, Category = "Blackboard")
	FGameplayTag CurrentTurnKey;

	UPROPERTY(EditDefaultsOnly, Category = "Blackboard")
	FGameplayTag CardsKey;

	UPROPERTY(EditDefaultsOnly, Category = "Blackboard")
	FGameplayTag HumanPlayerHandKey;
	
	UPROPERTY(EditDefaultsOnly, Category = "Blackboard")
	FGameplayTag DiscardNumKey;
	
	UPROPERTY(EditDefaultsOnly, Category = "Blackboard")
	FGameplayTag StatusObjectKey;
	
private:
	
	EPokerPlayer CurrentTurn;
	
	UPROPERTY()
	URFPokerStatus* CurrentStatusObject;
};

UCLASS(Blueprintable, BlueprintType)
class ROYALFLUSH_API URFBettingState : public URFPokerState
{
	GENERATED_BODY()

protected:
	
	virtual void OnActivate() override;
	
	virtual void OnDeactivate() override;
	
private:
	
	void SetTurn(EPokerPlayer Player);
	
	void PlayNPCTurn();
	
	UFUNCTION()
	void OnPlayerPassed(const FGameplayTag& Key);
	
	UFUNCTION()
	void OnPlayerFolded(const FGameplayTag& Key);
	
	UFUNCTION()
	void OnPlayerDoubleDowned(const FGameplayTag& Key);

private:
	
	UPROPERTY(EditDefaultsOnly, Category = "Default")
	float NPCTurnDelay;
	
	UPROPERTY(EditDefaultsOnly, Category = "Default")
	float NPCPlayDelay;
	
	UPROPERTY(EditDefaultsOnly, Category = "Default")
	float HumanTurnDelay;
	
	UPROPERTY(EditDefaultsOnly, Category = "Default")
	float EndStateDelay;
	
	UPROPERTY(EditDefaultsOnly, Category = "Blackboard")
	FGameplayTag PoolMoneyKey;
	
	UPROPERTY(EditDefaultsOnly, Category = "Blackboard")
	FGameplayTag PotMoneyKey;
	
	UPROPERTY(EditDefaultsOnly, Category = "Blackboard")
	FGameplayTag ScoreMultiplierKey;
	
	UPROPERTY(EditDefaultsOnly, Category = "Blackboard")
	FGameplayTag CurrentTurnKey;

	UPROPERTY(EditDefaultsOnly, Category = "Blackboard")
	FGameplayTag HumanMoneyKey;
	
	UPROPERTY(EditDefaultsOnly, Category = "Blackboard")
	FGameplayTag PassStatusKey;
	
	UPROPERTY(EditDefaultsOnly, Category = "Blackboard")
	FGameplayTag FoldStatusKey;
	
	UPROPERTY(EditDefaultsOnly, Category = "Blackboard")
	FGameplayTag DoubleDownStatusKey;
	
	UPROPERTY(EditDefaultsOnly, Category = "Blackboard")
	FGameplayTag NPCMoneyKey;
	
	UPROPERTY(EditDefaultsOnly, Category = "Blackboard")
	FGameplayTag StatusObjectKey;
	
private:
	
	EPokerPlayer CurrentPlayer;
	
	UPROPERTY()
	URFPokerStatus* CurrentStatusObject;
};

UCLASS(Blueprintable, BlueprintType)
class ROYALFLUSH_API URFRevealState : public URFPokerState
{
	GENERATED_BODY()

protected:

	virtual void OnActivate() override;
	
	EPokerRankComparision CompareFirstToSecond(URFHand* First, URFHand* Second);
	
private:
	
	UPROPERTY(EditDefaultsOnly)
	float EndStateDelay;
	
	UPROPERTY(EditDefaultsOnly, Category = "Blackboard")
	FGameplayTag HumanPlayerHandKey;
	
	UPROPERTY(EditDefaultsOnly, Category = "Blackboard")
	FGameplayTag NPCHandKey;
	
	UPROPERTY(EditDefaultsOnly, Category = "Blackboard")
	FGameplayTag PotMoneyKey;
	
	UPROPERTY(EditDefaultsOnly, Category = "Blackboard")
	FGameplayTag HumanMoneyKey;
	
	UPROPERTY(EditDefaultsOnly, Category = "Blackboard")
	FGameplayTag NPCMoneyKey;
	
private:
	
	UPROPERTY()
	TArray<URFPokerHandRuleBase*> Rules;
	
	bool bRulesInit = false;
};

UCLASS(Blueprintable, BlueprintType)
class ROYALFLUSH_API URFEndOfRoundState : public URFPokerState
{
	GENERATED_BODY()

protected:

	virtual void OnActivate() override;
	
protected:
	
	UPROPERTY(EditDefaultsOnly, Category = "Blackboard")
	FGameplayTag HumanMoneyKey;
	
	UPROPERTY(EditDefaultsOnly, Category = "Blackboard")
	FGameplayTag NPCMoneyKey;
	
	UPROPERTY(EditDefaultsOnly, Category = "Blackboard")
	FGameplayTag PoolMoneyKey;
	
	UPROPERTY(EditDefaultsOnly, Category = "Blackboard")
	FGameplayTag WinningPlayerKey;
	
	UPROPERTY(EditDefaultsOnly, Category = "Blackboard")
	FGameplayTag RoundNumKey;
	
	UPROPERTY(EditDefaultsOnly, Category = "Blackboard")
	FGameplayTag RoundRestartKey;
	
	UPROPERTY(EditDefaultsOnly, Category = "Blackboard")
	FGameplayTag GameEndStatusKey;
	
	UPROPERTY(EditDefaultsOnly, Category = "Blackboard")
	float EndStateDelay;
	
	UPROPERTY(EditDefaultsOnly, Category = "Blackboard")
	FGameplayTag StatusObjectKey;

private:
	
	UPROPERTY()
	URFPokerStatus* CurrentStatusObject;
};
