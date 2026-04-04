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
	
protected:
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnUpdateUI();
	
};

UCLASS(Blueprintable, BlueprintType)
class ROYALFLUSH_API URFPokerBeginState : public URFPokerState
{
	GENERATED_BODY()
	
protected:
	
	virtual void OnActivate() override;
	
	UFUNCTION(BlueprintImplementableEvent)
	void BP_SwitchToPokerCamera();
	
	UFUNCTION(BlueprintImplementableEvent)
	void BP_EnablePokerUI();
	
private:
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag PoolMoneyKey;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag RoundNumKey;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag CurrentPlayerIndexKey;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag ScoreMultiplierKey;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag CardsKey;
};

UCLASS(Blueprintable, BlueprintType)
class ROYALFLUSH_API URFPokerDealingState : public URFPokerState
{
	GENERATED_BODY()
	
protected:
	
	virtual void OnActivate() override;
	
private:

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag PotMoneyKey;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag CardsKey;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag PlayerHandKey;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag NPCHandKey;
};

UCLASS(Blueprintable, BlueprintType)
class ROYALFLUSH_API URFPokerDiscardingState : public URFPokerState
{
	GENERATED_BODY()

public:
	
	virtual void OnActivate() override;

protected:
	
	UFUNCTION(BlueprintImplementableEvent)
	void BP_ShowPassDiscardUI();

private:
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag PlayerDiscardNumKey;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag NPCDiscardNumKey;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag PlayerPassedKey;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag NPCPassedKey;
};

UCLASS(Blueprintable, BlueprintType)
class ROYALFLUSH_API URFBettingState : public URFPokerState
{
	GENERATED_BODY()

public:
	
	virtual void OnActivate() override;
	
private:
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag PoolMoneyKey;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag PotMoneyKey;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag ScoreMultiplierKey;

};

UCLASS(Blueprintable, BlueprintType)
class ROYALFLUSH_API URFCards : public UObject
{
	GENERATED_BODY()

public:
	
	URFCards();
	
	UFUNCTION(BlueprintCallable)
	void Shuffle();
	
	UFUNCTION(BlueprintCallable)
	TArray<int> NewHand();
	
	UFUNCTION(BlueprintCallable)
	void Reset();

private:
	
	TArray<int> Cards;
	int LastHandEndIndex = -1;
};

UCLASS(NotBlueprintable, NotBlueprintType)
class ROYALFLUSH_API URFHand : public UObject
{
	GENERATED_BODY()

public:
	
	void SetHand(const TArray<int>& NewHand);

private:
	
	TArray<int> Cards;
	
};