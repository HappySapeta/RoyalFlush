// Copyright (c) 2026 VINNIE BRIGHTEY, FELICITY ZABAVA, ARTHUR NORTH, JOSH BENNETTS, LEWIS TAIT, KYLE MURRAY, HOLLY ALBERT, ANUPAM SAHU, ARAMINTA MCDIARMID. All rights reserved.

#pragma once
#include "RFPokerRules.generated.h"

UCLASS()
class ROYALFLUSH_API URFPokerHandRuleBase : public UObject
{
	GENERATED_BODY()
public:
	
	virtual ~URFPokerHandRuleBase() = default;
	virtual bool Test(const TArray<int>& Hand) const PURE_VIRTUAL (URFPokerHandRuleBase::Test, return false; );
	virtual int Compare(const TArray<int>& FirstHand, const TArray<int>& SecondHand) const PURE_VIRTUAL (URFPokerHandRuleBase::Compare, return 0; );
};

UCLASS()
class ROYALFLUSH_API URFRoyalFlushRule : public URFPokerHandRuleBase 
{
	GENERATED_BODY()
public:
	virtual bool Test(const TArray<int>& Hand) const override;
	virtual int Compare(const TArray<int>& FirstHand, const TArray<int>& SecondHand) const override;
};

UCLASS()
class ROYALFLUSH_API URFStraightFlushRule : public URFPokerHandRuleBase 
{
	GENERATED_BODY()
public:
	virtual bool Test(const TArray<int>& Hand) const override;
	virtual int Compare(const TArray<int>& FirstHand, const TArray<int>& SecondHand) const override;
};

UCLASS()
class ROYALFLUSH_API URFFourOfAKindRule : public URFPokerHandRuleBase 
{
	GENERATED_BODY()
public:
	virtual bool Test(const TArray<int>& Hand) const override;
	virtual int Compare(const TArray<int>& FirstHand, const TArray<int>& SecondHand) const override;
};

UCLASS()
class ROYALFLUSH_API URFFullHouseRule : public URFPokerHandRuleBase 
{
	GENERATED_BODY()
public:
	virtual bool Test(const TArray<int>& Hand) const override;
	virtual int Compare(const TArray<int>& FirstHand, const TArray<int>& SecondHand) const override;
};

UCLASS()
class ROYALFLUSH_API URFFlushRule : public URFPokerHandRuleBase 
{
	GENERATED_BODY()
public:
	virtual bool Test(const TArray<int>& Hand) const override;
	virtual int Compare(const TArray<int>& FirstHand, const TArray<int>& SecondHand) const override;
};

UCLASS()
class ROYALFLUSH_API URFStraightRule : public URFPokerHandRuleBase 
{
	GENERATED_BODY()
public:
	virtual bool Test(const TArray<int>& Hand) const override;
	virtual int Compare(const TArray<int>& FirstHand, const TArray<int>& SecondHand) const override;
};
UCLASS()
class ROYALFLUSH_API URFThreeOfAKindRule : public URFPokerHandRuleBase 
{
	GENERATED_BODY()
public:
	virtual bool Test(const TArray<int>& Hand) const override;
	virtual int Compare(const TArray<int>& FirstHand, const TArray<int>& SecondHand) const override;
};

UCLASS()
class ROYALFLUSH_API URFTwoPairRule : public URFPokerHandRuleBase 
{
	GENERATED_BODY()
public:
	virtual bool Test(const TArray<int>& Hand) const override;
	virtual int Compare(const TArray<int>& FirstHand, const TArray<int>& SecondHand) const override;
};

UCLASS()
class ROYALFLUSH_API URFOnePairRule : public URFPokerHandRuleBase 
{
	GENERATED_BODY()
public:
	virtual bool Test(const TArray<int>& Hand) const override;
	virtual int Compare(const TArray<int>& FirstHand, const TArray<int>& SecondHand) const override;
};

UCLASS()
class ROYALFLUSH_API URFHighCardRule : public URFPokerHandRuleBase 
{
	GENERATED_BODY()
public:
	virtual bool Test(const TArray<int>& Hand) const override;
	virtual int Compare(const TArray<int>& FirstHand, const TArray<int>& SecondHand) const override;
};