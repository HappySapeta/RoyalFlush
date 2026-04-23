// Copyright (c) 2026 VINNIE BRIGHTEY, FELICITY ZABAVA, ARTHUR NORTH, JOSH BENNETTS, LEWIS TAIT, KYLE MURRAY, HOLLY ALBERT, ANUPAM SAHU, ARAMINTA MCDIARMID. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "StateMachine/RpState.h"
#include "RFPokerStates.generated.h"

UENUM(BlueprintType)
enum class EPokerPlayer : uint8
{
	NPC,
	Human
};

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
	FGameplayTag CurrentPlayerIndexKey;
	
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
};

UCLASS(Blueprintable, BlueprintType)
class ROYALFLUSH_API URFPokerDiscardingState : public URFPokerState
{
	GENERATED_BODY()
	
protected:

	virtual void OnActivate() override;
	
	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnHumanTurn();
	
	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnTurnChanged(EPokerPlayer Player);
	
	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnNPCPlayed();
	
	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnNPCDiscard();
	
	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnNPCPass();
	
private:

	void SetTurn(EPokerPlayer Player);
	void PlayNPCTurn();
	void NPCDiscard();
	
	UFUNCTION()
	void HandleDiscardRequested(const FGameplayTag& Key);
	
private:
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag DiscardStatusKey;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag PassStatusKey;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag DiscardedHandKey;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag CardsKey;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag NPCHandKey;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag HumanHandKey;
	
	EPokerPlayer CurrentPlayer;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag DiscardNumKey;
};

UCLASS(Blueprintable, BlueprintType)
class ROYALFLUSH_API URFBettingState : public URFPokerState
{
	GENERATED_BODY()

protected:
	
	virtual void OnActivate() override;
	
	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnBettingStarted();

	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnTurnChanged(EPokerPlayer Player);
	
	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnHumanFold();
	
	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnNPCPass();
	
	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnHumanPass();

	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnNPCFold();
	
	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnHumanPlayerTurn();
	
private:
	
	void EndTurn();
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
	
private:
	
	EPokerPlayer CurrentPlayer;
};

UCLASS()
class ROYALFLUSH_API URFRevealState : public URFPokerState
{
	GENERATED_BODY()

protected:

	virtual void OnActivate() override;
	
	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnRevealHands();
	
private:
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag HumanPlayerHandKey;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag NPCHandKey;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag RankedHandsKey;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag PotMoneyKey;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag HumanMoneyKey;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag NPCMoneyKey;
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
