// Copyright (c) 2026 VINNIE BRIGHTEY, FELICITY ZABAVA, ARTHUR NORTH, JOSH BENNETTS, LEWIS TAIT, KYLE MURRAY, HOLLY ALBERT, ANUPAM SAHU, ARAMINTA MCDIARMID. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "RoyalFlush/Journal/RFClueDataAsset.h"
#include "RFJournalClueWidget.generated.h"

class UImage;
/**
 * 
 */
UCLASS()
class ROYALFLUSH_API URFJournalClueWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION(BlueprintCallable)
	void SetData(FRFClue ClueData);
	
	UFUNCTION(BlueprintCallable)
	const FRFClue& GetData();
	
protected:
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ClueImage;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ClueDescriptionText;
	
	UPROPERTY(BlueprintReadOnly)
	FRFClue ClueData;
};
