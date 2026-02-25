// Copyright (c) 2026 VINNIE BRIGHTEY, FELICITY ZABAVA, ARTHUR NORTH, JOSH BENNETTS, LEWIS TAIT, KYLE MURRAY, HOLLY ALBERT, ANUPAM SAHU, ARAMINTA MCDIARMID. All rights reserved.

#include "RFDialogueComponent.h"

void URFDialogueComponent::StartInteraction(const UDlgContext* Context, const URFDialogueComponent* Other)
{
	DialogueContext = Context;
	OtherParticipant = Other;
	OnDialogueStartedEvent.Broadcast();
}

void URFDialogueComponent::EndInteraction(const UDlgContext* Context, const URFDialogueComponent* Other)
{
	DialogueContext = nullptr;
	OtherParticipant = nullptr;
	OnDialogueEndedEvent.Broadcast();
}
