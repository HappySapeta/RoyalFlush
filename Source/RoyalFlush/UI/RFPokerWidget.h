// Copyright (c) 2026 VINNIE BRIGHTEY, FELICITY ZABAVA, ARTHUR NORTH, JOSH BENNETTS, LEWIS TAIT, KYLE MURRAY, HOLLY ALBERT, ANUPAM SAHU, ARAMINTA MCDIARMID. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "RoyalFlush/Poker/RFPokerTypes.h"
#include "RFPokerWidget.generated.h"

class UWidgetSwitcher;
class UOverlay;
class URFPokerHandWidget;
class UButton;
class URFHand;
class URpStateMachineBlackboardBase;
/**
 * 
 */
UCLASS()
class ROYALFLUSH_API URFPokerWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintImplementableEvent)
	void OnHandUpdate(const FGameplayTag& Key);
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnOpponentHandUpdate(const FGameplayTag& Key);
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnTurnChanged(const FGameplayTag& Key);
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnStateChanged(const FGameplayTag& Key);

	UFUNCTION(BlueprintImplementableEvent)
	void OnTurnPassed(const FGameplayTag& Key);

	UFUNCTION(BlueprintImplementableEvent)
	void OnDoubleDowned(const FGameplayTag& Key);

	UFUNCTION(BlueprintImplementableEvent)
	void OnTurnFolded(const FGameplayTag& Key);
	
	UFUNCTION(BlueprintCallable)
	void SetBlackboard(URpStateMachineBlackboardBase* NewBlackboard);
	
protected:

	UFUNCTION()
	void HandlePlayerUnSelectedCard(int Index);
	
	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void HandlePlayerSelectedCard(int Index);

	UFUNCTION(BlueprintCallable)
	void HandlePlayerPressedDiscard();
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnBlackboardSet();
	
protected:
	
	UPROPERTY(BlueprintReadOnly)
	URpStateMachineBlackboardBase* Blackboard;
	
	UPROPERTY(BlueprintReadWrite)
	TSet<int> DiscardedCardIndices;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag HumanPlayerHandKey;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag DiscardedHandKey;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<URFPokerHandWidget> PokerHandWidget;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<URFPokerHandWidget> OpponentHandWidget;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> OverlaySwitcher;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag CurrentTurnKey;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag CurrentStateKey;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag PassStatusKey;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag DoubleDownStatusKey;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag FoldStatusKey;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag NPCHandKey;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag DiscardNumKey;

private:
	
	UPROPERTY()
	URFHand* DiscardedHand;
	
	EPokerState CurrentState;
};
