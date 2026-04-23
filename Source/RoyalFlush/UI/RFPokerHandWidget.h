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

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void UpdateHand(URFHand* NewHand);
	
protected:

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
	
	virtual void NativeConstruct() override;

public:
	
	UPROPERTY(BlueprintAssignable)
	FOnCardSelectedDelegate OnCardSelectedEvent;
	
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
};
