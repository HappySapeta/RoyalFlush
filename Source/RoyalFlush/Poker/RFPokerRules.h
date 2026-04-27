// Copyright (c) 2026 VINNIE BRIGHTEY, FELICITY ZABAVA, ARTHUR NORTH, JOSH BENNETTS, LEWIS TAIT, KYLE MURRAY, HOLLY ALBERT, ANUPAM SAHU, ARAMINTA MCDIARMID. All rights reserved.

#pragma once
#include "RFPokerRules.generated.h"

UENUM()
enum class EPokerRankComparision : uint8
{
	LOWER = 0,
	SAME = 1,
	HIGHER = 2
};

UCLASS()
class ROYALFLUSH_API URFPokerHandRuleBase : public UObject
{
	GENERATED_BODY()
public:
	
	virtual FString GetRuleName() const PURE_VIRTUAL (URFPokerHandRuleBase::GetRuleName, return TEXT(""); ); 
	virtual ~URFPokerHandRuleBase() = default;
	virtual bool Test(const TArray<int>& Hand) const PURE_VIRTUAL (URFPokerHandRuleBase::Test, return false; );
	virtual EPokerRankComparision Compare(const TArray<int>& FirstHand, const TArray<int>& SecondHand) const PURE_VIRTUAL (URFPokerHandRuleBase::Compare, return EPokerRankComparision::SAME; );
};

UCLASS()
class ROYALFLUSH_API URFRoyalFlushRule : public URFPokerHandRuleBase 
{
	GENERATED_BODY()
public:
	virtual FString GetRuleName() const override { return TEXT("Royal Flush"); } ;
	virtual bool Test(const TArray<int>& Hand) const override;
	virtual EPokerRankComparision Compare(const TArray<int>& FirstHand, const TArray<int>& SecondHand) const override;
};

UCLASS()
class ROYALFLUSH_API URFStraightFlushRule : public URFPokerHandRuleBase 
{
	GENERATED_BODY()
public:

	virtual FString GetRuleName() const override { return TEXT("Straight Flush"); } ;
	virtual bool Test(const TArray<int>& Hand) const override;
	virtual EPokerRankComparision Compare(const TArray<int>& FirstHand, const TArray<int>& SecondHand) const override;
};

UCLASS()
class ROYALFLUSH_API URFFourOfAKindRule : public URFPokerHandRuleBase 
{
	GENERATED_BODY()
public:

	virtual FString GetRuleName() const override { return TEXT("Four-of-A-kind"); } ;
	virtual bool Test(const TArray<int>& Hand) const override;
	virtual EPokerRankComparision Compare(const TArray<int>& FirstHand, const TArray<int>& SecondHand) const override;
};

UCLASS()
class ROYALFLUSH_API URFFullHouseRule : public URFPokerHandRuleBase 
{
	GENERATED_BODY()
public:

	virtual FString GetRuleName() const override { return TEXT("Full House"); } ;
	virtual bool Test(const TArray<int>& Hand) const override;
	virtual EPokerRankComparision Compare(const TArray<int>& FirstHand, const TArray<int>& SecondHand) const override;
};

UCLASS()
class ROYALFLUSH_API URFFlushRule : public URFPokerHandRuleBase 
{
	GENERATED_BODY()
public:

	virtual FString GetRuleName() const override { return TEXT("Flush"); } ;
	virtual bool Test(const TArray<int>& Hand) const override;
	virtual EPokerRankComparision Compare(const TArray<int>& FirstHand, const TArray<int>& SecondHand) const override;
};

UCLASS()
class ROYALFLUSH_API URFStraightRule : public URFPokerHandRuleBase 
{
	GENERATED_BODY()
public:

	virtual FString GetRuleName() const override { return TEXT("Straight"); } ;
	virtual bool Test(const TArray<int>& Hand) const override;
	virtual EPokerRankComparision Compare(const TArray<int>& FirstHand, const TArray<int>& SecondHand) const override;
};
UCLASS()
class ROYALFLUSH_API URFThreeOfAKindRule : public URFPokerHandRuleBase 
{
	GENERATED_BODY()
public:

	virtual FString GetRuleName() const override { return TEXT("Three-of-A-kind"); } ;
	virtual bool Test(const TArray<int>& Hand) const override;
	virtual EPokerRankComparision Compare(const TArray<int>& FirstHand, const TArray<int>& SecondHand) const override;
};

UCLASS()
class ROYALFLUSH_API URFTwoPairRule : public URFPokerHandRuleBase 
{
	GENERATED_BODY()
public:

	virtual FString GetRuleName() const override { return TEXT("Two Pair"); } ;
	virtual bool Test(const TArray<int>& Hand) const override;
	virtual EPokerRankComparision Compare(const TArray<int>& FirstHand, const TArray<int>& SecondHand) const override;
};

UCLASS()
class ROYALFLUSH_API URFOnePairRule : public URFPokerHandRuleBase 
{
	GENERATED_BODY()
public:

	virtual FString GetRuleName() const override { return TEXT("One Pair"); } ;
	virtual bool Test(const TArray<int>& Hand) const override;
	virtual EPokerRankComparision Compare(const TArray<int>& FirstHand, const TArray<int>& SecondHand) const override;
};

UCLASS()
class ROYALFLUSH_API URFHighCardRule : public URFPokerHandRuleBase 
{
	GENERATED_BODY()
public:

	virtual FString GetRuleName() const override { return TEXT("High Card"); } ;
	virtual bool Test(const TArray<int>& Hand) const override;
	virtual EPokerRankComparision Compare(const TArray<int>& FirstHand, const TArray<int>& SecondHand) const override;
};