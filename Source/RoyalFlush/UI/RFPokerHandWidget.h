// Copyright (c) 2026 VINNIE BRIGHTEY, FELICITY ZABAVA, ARTHUR NORTH, JOSH BENNETTS, LEWIS TAIT, KYLE MURRAY, HOLLY ALBERT, ANUPAM SAHU, ARAMINTA MCDIARMID. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RoyalFlush/Poker/RFPokerTypes.h"
#include "RFPokerHandWidget.generated.h"

class UButton;

/**
 * 
 */
UCLASS()
class ROYALFLUSH_API URFPokerHandWidget : public UUserWidget
{
	GENERATED_BODY()
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCardSelectedDelegate, int, Index);

public:

	UFUNCTION(BlueprintCallable)
	void SetAllowSelection(const bool bValue);
	
	UFUNCTION(BlueprintCallable)
	void UnSelectAll();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void UpdateHand(URFHand* NewHand);
	
protected:
	
	virtual void NativeConstruct() override;
	
private:
	
	void ToggleCardSelection(int Index);

	UFUNCTION()
	void OnFirstCardSelected();

	UFUNCTION()
	void OnSecondCardSelected();

	UFUNCTION()
	void OnThirdCardSelected();

	UFUNCTION()
	void OnFourthCardSelected();

	UFUNCTION()
	void OnFifthCardSelected();
	
public:
	
	UPROPERTY(BlueprintAssignable)
	FOnCardSelectedDelegate OnCardSelectedEvent;
	
	UPROPERTY(BlueprintAssignable)
	FOnCardSelectedDelegate OnCardUnSelectedEvent;

protected:
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> FirstCardButton;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> SecondCardButton;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> ThirdCardButton;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> FourthCardButton;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> FifthCardButton;
	
	UPROPERTY(BlueprintReadOnly, DisplayName = "IsSelected")
	TArray<bool> bIsSelected;
	
private:
	
	UPROPERTY()
	TArray<UButton*> Buttons;
	
	bool bSelectionAllowed;
};
