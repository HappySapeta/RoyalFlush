// Copyright (c) 2026 VINNIE BRIGHTEY, FELICITY ZABAVA, ARTHUR NORTH, JOSH BENNETTS, LEWIS TAIT, KYLE MURRAY, HOLLY ALBERT, ANUPAM SAHU, ARAMINTA MCDIARMID. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "RFPokerWidget.generated.h"

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
	
	
	UFUNCTION(BlueprintCallable)
	void SetBlackboard(URpStateMachineBlackboardBase* NewBlackboard);
	
protected:
	
	virtual void NativeConstruct() override;
	
	UFUNCTION(BlueprintCallable)
	void HandlePlayerPressedDiscard();
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnBlackboardSet();
	
protected:
	
	UPROPERTY(BlueprintReadOnly)
	URpStateMachineBlackboardBase* Blackboard;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag HumanPlayerHandKey;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag CardsKey;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag DiscardedHandKey;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag DiscardNumKey;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag DiscardStatusKey;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> DiscardingState_DiscardButton;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> DiscardingState_PassButton;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> BettingState_PassButton;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> BettingState_DoubleDownButton;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> BettingState_FoldButton;
	
	UPROPERTY(BlueprintReadWrite)
	TSet<int> DiscardedCardIndices;

private:
	
	UPROPERTY()
	URFHand* DiscardedHand;
};
