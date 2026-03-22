// Copyright (c) 2026 VINNIE BRIGHTEY, FELICITY ZABAVA, ARTHUR NORTH, JOSH BENNETTS, LEWIS TAIT, KYLE MURRAY, HOLLY ALBERT, ANUPAM SAHU, ARAMINTA MCDIARMID. All rights reserved.

#include "RFGameMode.h"

#include "StateMachine/RpStateMachineComponent.h"

ARFGameMode::ARFGameMode()
{
	PokerStateMachine = CreateDefaultSubobject<URpStateMachineComponent>(TEXT("PokerStateMachine"));
}
