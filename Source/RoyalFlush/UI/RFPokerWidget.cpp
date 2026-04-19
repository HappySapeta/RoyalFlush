// Copyright (c) 2026 VINNIE BRIGHTEY, FELICITY ZABAVA, ARTHUR NORTH, JOSH BENNETTS, LEWIS TAIT, KYLE MURRAY, HOLLY ALBERT, ANUPAM SAHU, ARAMINTA MCDIARMID. All rights reserved.

#include "RFPokerWidget.h"

void URFPokerWidget::SetBlackboard(const URpStateMachineBlackboardBase* NewBlackboard)
{
	Blackboard = NewBlackboard;
	OnBlackboardSet();
}
