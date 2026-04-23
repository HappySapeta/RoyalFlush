// Copyright (c) 2026 VINNIE BRIGHTEY, FELICITY ZABAVA, ARTHUR NORTH, JOSH BENNETTS, LEWIS TAIT, KYLE MURRAY, HOLLY ALBERT, ANUPAM SAHU, ARAMINTA MCDIARMID. All rights reserved.

#pragma once

struct PokerHandRuleBase
{
public:
	
	virtual ~PokerHandRuleBase() = default;
	virtual bool Test(const TArray<int>& Hand) = 0;
	virtual int Compare(const TArray<int>& FirstHand, const TArray<int>& SecondHand) = 0;
};

struct RoyalFlushRule : public PokerHandRuleBase 
{
public:
	virtual bool Test(const TArray<int>& Hand) override;
	virtual int Compare(const TArray<int>& FirstHand, const TArray<int>& SecondHand) override;
};

struct StraightFlushRule : public PokerHandRuleBase 
{
public:
	virtual bool Test(const TArray<int>& Hand) override;
	virtual int Compare(const TArray<int>& FirstHand, const TArray<int>& SecondHand) override;
};

struct FourOfAKindRule : public PokerHandRuleBase 
{
public:
	virtual bool Test(const TArray<int>& Hand) override;
	virtual int Compare(const TArray<int>& FirstHand, const TArray<int>& SecondHand) override;
};

struct FullHouseRule : public PokerHandRuleBase 
{
public:
	virtual bool Test(const TArray<int>& Hand) override;
	virtual int Compare(const TArray<int>& FirstHand, const TArray<int>& SecondHand) override;
};

struct FlushRule : public PokerHandRuleBase 
{
public:
	virtual bool Test(const TArray<int>& Hand) override;
	virtual int Compare(const TArray<int>& FirstHand, const TArray<int>& SecondHand) override;
};

struct StraightRule : public PokerHandRuleBase 
{
public:
	virtual bool Test(const TArray<int>& Hand) override;
	virtual int Compare(const TArray<int>& FirstHand, const TArray<int>& SecondHand) override;
};

struct ThreeOfAKindRule : public PokerHandRuleBase 
{
public:
	virtual bool Test(const TArray<int>& Hand) override;
	virtual int Compare(const TArray<int>& FirstHand, const TArray<int>& SecondHand) override;
};

struct TwoPairRule : public PokerHandRuleBase 
{
public:
	virtual bool Test(const TArray<int>& Hand) override;
	virtual int Compare(const TArray<int>& FirstHand, const TArray<int>& SecondHand) override;
};

struct OnePairRule : public PokerHandRuleBase 
{
public:
	virtual bool Test(const TArray<int>& Hand) override;
	virtual int Compare(const TArray<int>& FirstHand, const TArray<int>& SecondHand) override;
};

struct HighCardRule : public PokerHandRuleBase 
{
public:
	virtual bool Test(const TArray<int>& Hand) override;
	virtual int Compare(const TArray<int>& FirstHand, const TArray<int>& SecondHand) override;
};