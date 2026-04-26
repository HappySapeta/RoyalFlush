// Copyright (c) 2026 VINNIE BRIGHTEY, FELICITY ZABAVA, ARTHUR NORTH, JOSH BENNETTS, LEWIS TAIT, KYLE MURRAY, HOLLY ALBERT, ANUPAM SAHU, ARAMINTA MCDIARMID. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "StateMachine/RpState.h"
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
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag OwningActorKey;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag CurrentStateKey;
	
	UPROPERTY(EditDefaultsOnly)
	EPokerState CurrentState;
};

UCLASS(Blueprintable, BlueprintType)
class ROYALFLUSH_API URFPokerBeginState : public URFPokerState
{
	GENERATED_BODY()
	
protected:

	virtual void OnActivate() override;
	
	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnBeginPoker();
	
private:
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag PoolMoneyKey;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag RoundNumKey;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag ScoreMultiplierKey;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag CardsKey;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag RoundRestartKey;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag GameEndStatusKey;
};

UCLASS(Blueprintable, BlueprintType)
class ROYALFLUSH_API URFPokerDealingState : public URFPokerState
{
	GENERATED_BODY()
	
protected:

	virtual void OnActivate() override;
	
	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnDealPoker();

private:

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag PotMoneyKey;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag CardsKey;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag HumanPlayerHandKey;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag NPCHandKey;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag RoundNumKey;
};

UCLASS(Blueprintable, BlueprintType)
class ROYALFLUSH_API URFPokerDiscardingState : public URFPokerState
{
	GENERATED_BODY()
	
protected:

	virtual void OnActivate() override;
	
	virtual void OnDeactivate() override;

	UFUNCTION()
	void HandleDiscardRequested(const FGameplayTag& Key);

private:

	void SetTurn(EPokerPlayer Player);

	void PlayNPCTurn();
	
private:

	UPROPERTY(EditDefaultsOnly)
	float NPCTurnDelay;
	
	UPROPERTY(EditDefaultsOnly)
	float HumanTurnDelay;
	
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
	
	EPokerPlayer CurrentTurn;
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
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag PoolMoneyKey;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag PotMoneyKey;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag ScoreMultiplierKey;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag CurrentTurnKey;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag HumanMoneyKey;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag PassStatusKey;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag FoldStatusKey;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag DoubleDownStatusKey;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag NPCMoneyKey;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag RoundEndKey;
	
	UPROPERTY(EditDefaultsOnly)
	float NPCTurnDelay;
	
	UPROPERTY(EditDefaultsOnly)
	float HumanTurnDelay;
	
private:
	
	EPokerPlayer CurrentPlayer;
};

UCLASS()
class ROYALFLUSH_API URFRevealState : public URFPokerState
{
	GENERATED_BODY()

protected:

	virtual void OnActivate() override;
	
	EPokerRankComparision CompareFirstToSecond(URFHand* First, URFHand* Second);
	
private:
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag HumanPlayerHandKey;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag NPCHandKey;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag PotMoneyKey;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag HumanMoneyKey;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag NPCMoneyKey;
	
private:
	
	UPROPERTY()
	TArray<URFPokerHandRuleBase*> Rules;
	
	bool bRulesInit = false;
};

UCLASS()
class ROYALFLUSH_API URFEndOfRoundState : public URFPokerState
{
	GENERATED_BODY()

protected:

	virtual void OnActivate() override;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag HumanMoneyKey;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag NPCMoneyKey;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag PoolMoneyKey;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag WinningPlayerKey;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag RoundNumKey;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag RoundRestartKey;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag GameEndStatusKey;
};
