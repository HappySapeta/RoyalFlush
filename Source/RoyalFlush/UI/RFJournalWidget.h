// Copyright (c) 2026 VINNIE BRIGHTEY, FELICITY ZABAVA, ARTHUR NORTH, JOSH BENNETTS, LEWIS TAIT, KYLE MURRAY, HOLLY ALBERT, ANUPAM SAHU, ARAMINTA MCDIARMID. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ScrollBox.h"
#include "RoyalFlush/Journal/RFJournalComponent.h"
#include "RFJournalWidget.generated.h"

class URFJournalPageWidget;
/**
 * 
 */
UCLASS()
class ROYALFLUSH_API URFJournalWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	
	virtual void NativeConstruct() override;

	UFUNCTION()
	void HandleOnVisibilityChanged(ESlateVisibility InVisibility);

	void PopulateClueWidgets();
	
private:
	
	TArray<FRFClue> GetNewClues(const TArray<FRFClue>& Clues);
	
	URFJournalPageWidget* CreateNewPage();
	
protected:
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScrollBox> ScrollBox;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<URFJournalPageWidget> JournalPageWidgetClass;

private:
	
	TArray<FRFClue> CurrentClues;
	
	UPROPERTY()
	URFJournalPageWidget* LastJournalPage;
};
