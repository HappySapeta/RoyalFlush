// Copyright (c) 2026 VINNIE BRIGHTEY, FELICITY ZABAVA, ARTHUR NORTH, JOSH BENNETTS, LEWIS TAIT, KYLE MURRAY, HOLLY ALBERT, ANUPAM SAHU, ARAMINTA MCDIARMID. All rights reserved.


#include "RFCardTextureDataAsset.h"

UTexture2D* URFCardTextureDataAsset::GetImage(const int CardNumber)
{
	if (Textures.Contains(CardNumber))
	{
		return Textures[CardNumber].Image;
	}
	
	return nullptr;
}
