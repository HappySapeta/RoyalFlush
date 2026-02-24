// Copyright (c) 2026 VINNIE BRIGHTEY, FELICITY ZABAVA, ARTHUR NORTH, JOSH BENNETTS, LEWIS TAIT, KYLE MURRAY, HOLLY ALBERT, ANUPAM SAHU, ARAMINTA MCDIARMID. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Blueprint/UserWidget.h"

#include "DialogueOptionEntryWidget.generated.h"

UCLASS(BlueprintType)
class UDialogueListItem : public UObject
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDialogueChosenDelegate, int, OptionIndex);
	
public:

	UPROPERTY(BlueprintReadWrite)
	FText DialogueText;
	
	UPROPERTY(BlueprintReadWrite)
	int DialogueOptionIndex;
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnDialogueChosenDelegate OnDialogueChosen;
};

class UTextBlock;
/**
 * 
 */
UCLASS()
class ROYALFLUSH_API UDialogueOptionEntryWidget : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

protected:

	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> DialogueTextBlock;
};
