// Copyright (c) 2026 VINNIE BRIGHTEY, FELICITY ZABAVA, ARTHUR NORTH, JOSH BENNETTS, LEWIS TAIT, KYLE MURRAY, HOLLY ALBERT, ANUPAM SAHU, ARAMINTA MCDIARMID. All rights reserved.


#include "DialogueOptionEntryWidget.h"

#include "Components/TextBlock.h"

void UDialogueOptionEntryWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);
	if (UDialogueListItem* DialogueListItem = Cast<UDialogueListItem>(ListItemObject))
	{
		DialogueTextBlock->SetText(DialogueListItem->DialogueText);
	}
}
