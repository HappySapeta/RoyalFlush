// Copyright (c) 2026 VINNIE BRIGHTEY, FELICITY ZABAVA, ARTHUR NORTH, JOSH BENNETTS, LEWIS TAIT, KYLE MURRAY, HOLLY ALBERT, ANUPAM SAHU, ARAMINTA MCDIARMID. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "RFPokerRules.h"
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

UCLASS(BlueprintType)
class ROYALFLUSH_API URFPokerStatus : public UObject
{
	GENERATED_BODY()

public:
	
	void SetStatus(const FString Text);

protected:
	
	UPROPERTY(BlueprintReadOnly)
	FText StatusText;
};
