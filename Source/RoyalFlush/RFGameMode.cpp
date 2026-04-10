// Copyright (c) 2026 VINNIE BRIGHTEY, FELICITY ZABAVA, ARTHUR NORTH, JOSH BENNETTS, LEWIS TAIT, KYLE MURRAY, HOLLY ALBERT, ANUPAM SAHU, ARAMINTA MCDIARMID. All rights reserved.

#include "RFGameMode.h"

#include "Journal/RFJournalComponent.h"

ARFGameMode::ARFGameMode()
{
	JournalComponent = CreateDefaultSubobject<URFJournalComponent>(TEXT("JournalComponent"));
}
