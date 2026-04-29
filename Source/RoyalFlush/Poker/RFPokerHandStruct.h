// Copyright (c) 2026 VINNIE BRIGHTEY, FELICITY ZABAVA, ARTHUR NORTH, JOSH BENNETTS, LEWIS TAIT, KYLE MURRAY, HOLLY ALBERT, ANUPAM SAHU, ARAMINTA MCDIARMID. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Class.h"
#include "RFPokerHandStruct.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct ROYALFLUSH_API FRFPokerHandStruct : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PokerHand")
	FString Hand = "";
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PokerHand")
	TArray<int> Cards;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PokerHand")
	float PlayerChance = 0.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PokerHand")
	float NPCChance = 0.0f;
};
