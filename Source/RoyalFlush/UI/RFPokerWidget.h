// Copyright (c) 2026 VINNIE BRIGHTEY, FELICITY ZABAVA, ARTHUR NORTH, JOSH BENNETTS, LEWIS TAIT, KYLE MURRAY, HOLLY ALBERT, ANUPAM SAHU, ARAMINTA MCDIARMID. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RFPokerWidget.generated.h"

class URpStateMachineBlackboardBase;
/**
 * 
 */
UCLASS()
class ROYALFLUSH_API URFPokerWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable)
	void SetBlackboard(const URpStateMachineBlackboardBase* NewBlackboard);
	
protected:
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnBlackboardSet();
	
protected:
	
	UPROPERTY(BlueprintReadOnly)
	const URpStateMachineBlackboardBase* Blackboard;
};
