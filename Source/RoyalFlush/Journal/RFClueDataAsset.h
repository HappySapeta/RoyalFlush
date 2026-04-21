// Copyright (c) 2026 VINNIE BRIGHTEY, FELICITY ZABAVA, ARTHUR NORTH, JOSH BENNETTS, LEWIS TAIT, KYLE MURRAY, HOLLY ALBERT, ANUPAM SAHU, ARAMINTA MCDIARMID. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RFClueDataAsset.generated.h"

USTRUCT(BlueprintType)
struct FRFClue
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText Description;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UTexture2D> Image;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool IsStaged = false;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool IsReal = false;

	bool operator==(const FRFClue& Other) const
	{
		return Description.EqualTo(Other.Description);
	}
};

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class ROYALFLUSH_API URFClueDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION(BlueprintCallable)
	FRFClue GetData() const { return Data; }
	
protected:
	
	UPROPERTY(EditDefaultsOnly)
	FRFClue Data;
};
