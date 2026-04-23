// Copyright (c) 2026 VINNIE BRIGHTEY, FELICITY ZABAVA, ARTHUR NORTH, JOSH BENNETTS, LEWIS TAIT, KYLE MURRAY, HOLLY ALBERT, ANUPAM SAHU, ARAMINTA MCDIARMID. All rights reserved.

#pragma once

constexpr int BASE_SCORE_MULTIPLIER = 19;
constexpr int NUM_PLAYING_CARDS = 52;
constexpr int HAND_SIZE = 5;

#include "CoreMinimal.h"
#include "RFPokerHands.generated.h"

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

struct PokerHandRuleBase
{
public:
	
	virtual ~PokerHandRuleBase() = default;
	virtual bool Test(const URFHand* Hand) = 0;
	virtual int Compare(const URFHand* FirstHand, const URFHand* SecondHand) = 0;
};

struct RoyalFlushRule : public PokerHandRuleBase 
{
public:
	virtual bool Test(const URFHand* Hand) override;
	virtual int Compare(const URFHand* FirstHand, const URFHand* SecondHand) override;
};

struct StraightFlushRule : public PokerHandRuleBase 
{
public:
	virtual bool Test(const URFHand* Hand) override;
	virtual int Compare(const URFHand* FirstHand, const URFHand* SecondHand) override;
};

struct FourOfAKindRule : public PokerHandRuleBase 
{
public:
	virtual bool Test(const URFHand* Hand) override;
	virtual int Compare(const URFHand* FirstHand, const URFHand* SecondHand) override;
};

struct FullHouseRule : public PokerHandRuleBase 
{
public:
	virtual bool Test(const URFHand* Hand) override;
	virtual int Compare(const URFHand* FirstHand, const URFHand* SecondHand) override;
};

struct FlushRule : public PokerHandRuleBase 
{
public:
	virtual bool Test(const URFHand* Hand) override;
	virtual int Compare(const URFHand* FirstHand, const URFHand* SecondHand) override;
};

struct StraightRule : public PokerHandRuleBase 
{
public:
	virtual bool Test(const URFHand* Hand) override;
	virtual int Compare(const URFHand* FirstHand, const URFHand* SecondHand) override;
};

struct ThreeOfAKindRule : public PokerHandRuleBase 
{
public:
	virtual bool Test(const URFHand* Hand) override;
	virtual int Compare(const URFHand* FirstHand, const URFHand* SecondHand) override;
};

struct TwoPairRule : public PokerHandRuleBase 
{
public:
	virtual bool Test(const URFHand* Hand) override;
	virtual int Compare(const URFHand* FirstHand, const URFHand* SecondHand) override;
};

struct OnePairRule : public PokerHandRuleBase 
{
public:
	virtual bool Test(const URFHand* Hand) override;
	virtual int Compare(const URFHand* FirstHand, const URFHand* SecondHand) override;
};

struct HighCardRule : public PokerHandRuleBase 
{
public:
	virtual bool Test(const URFHand* Hand) override;
	virtual int Compare(const URFHand* FirstHand, const URFHand* SecondHand) override;
};