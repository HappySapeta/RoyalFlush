// Copyright (c) 2026 VINNIE BRIGHTEY, FELICITY ZABAVA, ARTHUR NORTH, JOSH BENNETTS, LEWIS TAIT, KYLE MURRAY, HOLLY ALBERT, ANUPAM SAHU, ARAMINTA MCDIARMID. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "RFJournalClueWidget.h"
#include "Blueprint/UserWidget.h"
#include "RoyalFlush/Journal/RFClueDataAsset.h"
#include "RFJournalPageWidget.generated.h"

class UUniformGridPanel;
/**
 * 
 */
UCLASS()
class ROYALFLUSH_API URFJournalPageWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION(BlueprintCallable)
	void AddClue(const FRFClue& ClueItem);
	
	UFUNCTION(BlueprintCallable)
	bool CanContainMoreClues();
	
protected:
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<URFJournalClueWidget> ClueWidgetClass;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUniformGridPanel> UniformGridPanel;
	
	UPROPERTY(EditDefaultsOnly)
	int ClueCapacity;
	
	UPROPERTY(EditDefaultsOnly)
	int NumColumns;
	
	UPROPERTY(EditDefaultsOnly)
	float MaxRandomTranslation;
	
	UPROPERTY(EditDefaultsOnly)
	float MaxRandomAngle;
};
