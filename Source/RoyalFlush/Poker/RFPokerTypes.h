// Copyright (c) 2026 VINNIE BRIGHTEY, FELICITY ZABAVA, ARTHUR NORTH, JOSH BENNETTS, LEWIS TAIT, KYLE MURRAY, HOLLY ALBERT, ANUPAM SAHU, ARAMINTA MCDIARMID. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "RFPokerHandStruct.h"
#include "RFPokerTypes.generated.h"

UENUM(BlueprintType)
enum class EPokerPlayer : uint8
{
	NPC,
	Human,
	None
};

UENUM(BlueprintType)
enum class EPokerState : uint8
{
	Begin,
	Dealing,
	Discarding,
	Betting,
	Reveal,
	EndOfRound
};

UCLASS(Blueprintable, BlueprintType)
class ROYALFLUSH_API URFCards : public UObject
{
	GENERATED_BODY()

public:

	URFCards();
	
	void SetSpawnData(const UDataTable* Data);

	UFUNCTION(BlueprintCallable)
	void Shuffle();
	
	int DrawCard();
	int DrawCard(const int SpecificCard);
	void DrawHand(const TArray<int>& Hand);
	bool IsHandDrawable(const TArray<int>& Hand);

	UFUNCTION(BlueprintCallable)
	TArray<int> NewHand(EPokerPlayer Player);

	UFUNCTION(BlueprintCallable)
	void Reset();
	void DebugLogCards();

	UFUNCTION(BlueprintCallable)
	void ReplaceDiscardedCards(TArray<int> CardIndicesToBeDiscarded, TArray<int>& TargetHand);

private:
	
	int LastHandEndIndex = -1;
	float PlayerChanceUpperLimit = 0.0f;
	float NPCChanceUpperLimit = 0.0f;
	TArray<int> Cards;
	TArray<TRange<float>> PlayerChances;
	TArray<TRange<float>> NPCChances;
	
	UPROPERTY()
	const UDataTable* DataTable;
};

UCLASS(BlueprintType)
class ROYALFLUSH_API URFHand : public UObject
{
	GENERATED_BODY()

public:

	const TArray<int>& GetCards() const;

	void SetCards(const TArray<int>& NewCards);

	bool Equals(const URFHand* Other) const;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<int> Cards;

};

UCLASS(BlueprintType)
class ROYALFLUSH_API URFPokerStatus : public UObject
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable)
	void ClearStatus();
	
	UFUNCTION(BlueprintCallable)
	void SetStatus(const FString Text);

protected:
	
	UPROPERTY(BlueprintReadOnly)
	FText StatusText;
};
