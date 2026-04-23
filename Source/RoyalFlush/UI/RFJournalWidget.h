// Copyright (c) 2026 VINNIE BRIGHTEY, FELICITY ZABAVA, ARTHUR NORTH, JOSH BENNETTS, LEWIS TAIT, KYLE MURRAY, HOLLY ALBERT, ANUPAM SAHU, ARAMINTA MCDIARMID. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
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
	
private:
	
	void PopulateClueWidgets();
	
	UFUNCTION()
	void HandleAcceptButtonClicked();
	
	UFUNCTION()
	void HandleOnVisibilityChanged(ESlateVisibility InVisibility);
	
private:
	
	TArray<FRFClue> GetNewClues(const TArray<FRFClue>& Clues);
	
	URFJournalPageWidget* CreateNewPage();
	
	UFUNCTION()
	void HandleClueStaged(FRFClue ClueData);
	
	UFUNCTION()
	void HandleClueUnStaged(FRFClue Clue);
	
protected:
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScrollBox> PageScroller;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScrollBox> CluesContainer;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> AcceptButton;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<URFJournalPageWidget> JournalPageWidgetClass;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> ClueWidgetClass;
	
private:
	
	TArray<FRFClue> CurrentClues;
	
	UPROPERTY()
	URFJournalPageWidget* LastJournalPage;
};
