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
	virtual void OnDeactivate() override;
	void ExecuteWithDelay(FTimerDelegate Callback, const float Delay);
	
protected:
	
	UPROPERTY(EditDefaultsOnly, Category = "Default")
	EPokerState CurrentState;
	
	UPROPERTY(EditDefaultsOnly, Category = "Blackboard")
	FGameplayTag OwningActorKey;
	
	UPROPERTY(EditDefaultsOnly, Category = "Blackboard")
	FGameplayTag CurrentStateKey;

	UPROPERTY(EditDefaultsOnly, Category = "Blackboard")
	FGameplayTag StatusObjectKey;

protected:
	
	UPROPERTY()
	URFPokerStatus* CurrentStatusObject;
};

UCLASS(Blueprintable, BlueprintType)
class ROYALFLUSH_API URFPokerBeginState : public URFPokerState
{
	GENERATED_BODY()
	
protected:

	virtual void OnActivate() override;
	
private:
	
	UPROPERTY(EditDefaultsOnly, Category = "Default")
	float InitialPoolMoney;
	
	UPROPERTY(EditDefaultsOnly, Category = "Blackboard")
	FGameplayTag PoolMoneyKey;
	
	UPROPERTY(EditDefaultsOnly, Category = "Blackboard")
	FGameplayTag RoundNumKey;
	
	UPROPERTY(EditDefaultsOnly, Category = "Blackboard")
	FGameplayTag CardsObjectKey;
	
	UPROPERTY(EditDefaultsOnly, Category = "Blackboard")
	FGameplayTag GameEndStatusKey;
	
	UPROPERTY(EditDefaultsOnly, Category = "Blackboard")
	FGameplayTag HumanMoneyKey;
	
	UPROPERTY(EditDefaultsOnly, Category = "Blackboard")
	FGameplayTag NPCMoneyKey;
	
	UPROPERTY(EditDefaultsOnly, Category = "Default")
	float EndStateDelay = 2.0f;
};

UCLASS(Blueprintable, BlueprintType)
class ROYALFLUSH_API URFPokerDealingState : public URFPokerState
{
	GENERATED_BODY()
	
protected:

	TArray<int> DebugDealHand(int Index);
	virtual void OnActivate() override;

private:
	
	UPROPERTY(EditDefaultsOnly, Category = "Default")
	const UDataTable* SpawnData;

	UPROPERTY(EditDefaultsOnly, Category = "Default")
	float DealingStateDelay;
	
	UPROPERTY(EditDefaultsOnly, Category = "Debug")
	int NPCDebugHand = -1;

	UPROPERTY(EditDefaultsOnly, Category = "Debug")
	int PlayerDebugHand = -1;

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
	
private:
	
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
	
	UPROPERTY(EditDefaultsOnly, Category = "Default")
	float NPCTurnDelay;
	
	UPROPERTY(EditDefaultsOnly, Category = "Default")
	float NPCPlayDelay;
	
	UPROPERTY(EditDefaultsOnly, Category = "Default")
	float HumanTurnDelay;
	
	UPROPERTY(EditDefaultsOnly)
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
	
	UPROPERTY(EditDefaultsOnly, Category = "Debug")
	bool bNPCAlwaysPass;
	
private:
	
	EPokerPlayer CurrentTurn;
};

UCLASS(Blueprintable, BlueprintType)
class ROYALFLUSH_API URFRevealState : public URFPokerState
{
	GENERATED_BODY()

protected:

	virtual void OnActivate() override;
	
	TPair<EPokerRankComparision, FString> CompareFirstToSecond(URFHand* First, URFHand* Second);
	
private:

	UPROPERTY(EditDefaultsOnly, Category = "Default")
	float DeclarationDelay;
	
	UPROPERTY(EditDefaultsOnly, Category = "Default")
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
	FGameplayTag GameEndStatusKey;
	
	UPROPERTY(EditDefaultsOnly, Category = "Default")
	float DeclarationDelay;
	
	UPROPERTY(EditDefaultsOnly, Category = "Default")
	int MaxNumRounds;
	
	UPROPERTY(EditDefaultsOnly, Category = "Default")
	float EndGameDelay;
};
