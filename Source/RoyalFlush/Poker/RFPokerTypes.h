// Copyright (c) 2026 VINNIE BRIGHTEY, FELICITY ZABAVA, ARTHUR NORTH, JOSH BENNETTS, LEWIS TAIT, KYLE MURRAY, HOLLY ALBERT, ANUPAM SAHU, ARAMINTA MCDIARMID. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "RFPokerTypes.generated.h"

UENUM(BlueprintType)
enum class EPokerPlayer : uint8
{
	NPC,
	Human
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

	UFUNCTION(BlueprintCallable)
	void Shuffle();

	UFUNCTION(BlueprintCallable)
	TArray<int> NewHand();

	UFUNCTION(BlueprintCallable)
	void Reset();

	int SwapCard(int Card);

	UFUNCTION(BlueprintCallable)
	void ReplaceDiscardedCards(TArray<int> CardIndicesToBeDiscarded, TArray<int>& TargetHand);

private:

	TArray<int> Cards;
	int LastHandEndIndex = -1;
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

class URFPokerHandRule;

UCLASS(Blueprintable, BlueprintType)
class URFRankedHands : public UObject
{
	GENERATED_BODY()

public:

	int GetRank(URFHand* Hand);
	bool IsFirstHigherThanSecond(URFHand* First, URFHand* Second);

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<URFHand*> RankedHands;
};