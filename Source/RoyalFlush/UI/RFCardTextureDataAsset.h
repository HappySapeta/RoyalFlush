// Copyright (c) 2026 VINNIE BRIGHTEY, FELICITY ZABAVA, ARTHUR NORTH, JOSH BENNETTS, LEWIS TAIT, KYLE MURRAY, HOLLY ALBERT, ANUPAM SAHU, ARAMINTA MCDIARMID. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RFCardTextureDataAsset.generated.h"

USTRUCT(BlueprintType)
struct FRFCardImageStruct
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UTexture2D> Image;
};

/**
 * 
 */
UCLASS(BlueprintType)
class ROYALFLUSH_API URFCardTextureDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TMap<int, FRFCardImageStruct> Textures;
	
};
