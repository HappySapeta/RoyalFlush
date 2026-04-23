// Copyright (c) 2026 VINNIE BRIGHTEY, FELICITY ZABAVA, ARTHUR NORTH, JOSH BENNETTS, LEWIS TAIT, KYLE MURRAY, HOLLY ALBERT, ANUPAM SAHU, ARAMINTA MCDIARMID. All rights reserved.

#include "RFPokerRules.h"
#include "RFPokerDeck.h"

constexpr int GET_RANK(const int Card)
{
	return Card % 13;
}
	
constexpr int GET_SUIT(const int Card)
{
	return Card / 13;
}

bool RoyalFlushRule::Test(const TArray<int>& Cards)
{
	bool bHasAce = false;
	bool bHasKing = false;
	bool bHasQueen = false;
	bool bHasJack = false;
	bool bHasTen = false;
	int CurrentSuit = GET_SUIT(Cards[0]);
	
	for (int Card : Cards)
	{
		if (GET_SUIT(Card) != CurrentSuit)
		{
			return false;
		}
		
		bHasAce = bHasAce || GET_RANK(Card) == GET_RANK(RF_ACE_DIAMONDS);
		bHasKing = bHasKing || GET_RANK(Card) == GET_RANK(RF_KING_DIAMONDS);
		bHasQueen = bHasQueen || GET_RANK(Card) == GET_RANK(RF_QUEEN_DIAMONDS);
		bHasJack = bHasJack || GET_RANK(Card) == GET_RANK(RF_JACK_DIAMONDS);
		bHasTen = bHasTen || GET_RANK(Card) == GET_RANK(RF_TEN_DIAMONDS);
	}
	
	return bHasAce && bHasKing && bHasQueen && bHasJack && bHasTen;
}

int RoyalFlushRule::Compare(const TArray<int>& FirstHand, const TArray<int>& SecondHand)
{
	return 0;
}

bool StraightFlushRule::Test(const TArray<int>& Cards)
{
	int CurrentSuit = GET_SUIT(Cards[0]);
	for (int Card : Cards)
	{
		if (GET_SUIT(Card) != CurrentSuit)
		{
			return false;
		}
	}
	
	int Highest = *Algo::MaxElement(Cards);
	bool bHasSecond = Cards.Contains(Highest - 1);
	bool bHasThird = Cards.Contains(Highest - 2);
	bool bHasFourth = Cards.Contains(Highest - 3);
	bool bHasFifth = Cards.Contains(Highest - 4);
	
	return bHasSecond && bHasThird && bHasFourth && bHasFifth;
}

int StraightFlushRule::Compare(const TArray<int>& FirstHand, const TArray<int>& SecondHand)
{
	auto SumOperation = [](const int A, const int B){ return A + B; };
	int FirstSum = Algo::Accumulate(FirstHand, 0, SumOperation);
	int SecondSum = Algo::Accumulate(SecondHand, 0, SumOperation);
	
	return FirstSum > SecondSum;
}

bool FourOfAKindRule::Test(const TArray<int>& Cards)
{
	TMap<int, int> KindCountMap;
	for (int Card : Cards)
	{
		int Kind = GET_RANK(Card);
		if (KindCountMap.Contains(Kind))
		{
			KindCountMap[Kind] += 1;
		}
		else
		{
			KindCountMap[Kind] = 1;
		}
	}
	
	for (const auto& [Kind, Count] : KindCountMap)
	{
		if (Count == 4)
		{
			return true;
		}
	}
	
	return false;
}

int FourOfAKindRule::Compare(const TArray<int>& FirstHand, const TArray<int>& SecondHand)
{
	TArray<int> FirstCards = FirstHand;
	TArray<int> SecondCards = SecondHand;
	
	auto Predicate = [](const int A, const int B)
	{
		return A > B;
	};
	
	Algo::Sort(FirstCards, Predicate);
	Algo::Sort(SecondCards, Predicate);
	
	TArray<int> Difference;
	for (int Index = 0; Index < FirstCards.Num(); ++Index)
	{
		int FirstRank = GET_RANK(FirstCards[Index]);
		int SecondRank = GET_RANK(SecondCards[Index]);
		
		if (FirstRank > SecondRank)
		{
			return 1;
		}
		else if (FirstRank < SecondRank)
		{
			return -1;
		}
	}
	
	return 0;
}

bool FullHouseRule::Test(const TArray<int>& Cards)
{
	TMap<int, int> KindCountMap;
	for (int Card : Cards)
	{
		int Kind = GET_RANK(Card);
		if (KindCountMap.Contains(Kind))
		{
			KindCountMap[Kind] += 1;
		}
		else
		{
			KindCountMap[Kind] = 1;
		}
	}
	
	bool HasThreeOfAKind = false;
	bool HasTwoOfAKind = false;
	for (const auto [Kind, Count] : KindCountMap)
	{
		if (Count == 3)
		{
			HasThreeOfAKind = true;
		}
		else if (Count == 2)
		{
			HasTwoOfAKind = true;
		}
	}
	
	return HasThreeOfAKind && HasTwoOfAKind;
}

int FullHouseRule::Compare(const TArray<int>& FirstHand, const TArray<int>& SecondHand)
{
	TArray<int> FirstCards = FirstHand;
	TArray<int> SecondCards = SecondHand;
	
	auto Predicate = [](const int A, const int B)
	{
		return A > B;
	};
	
	Algo::Sort(FirstCards, Predicate);
	Algo::Sort(SecondCards, Predicate);
	
	TArray<int> Difference;
	for (int Index = 0; Index < FirstCards.Num(); ++Index)
	{
		int FirstRank = GET_RANK(FirstCards[Index]);
		int SecondRank = GET_RANK(SecondCards[Index]);
		
		if (FirstRank > SecondRank)
		{
			return 1;
		}
		else if (FirstRank < SecondRank)
		{
			return -1;
		}
	}
	
	return 0;
}

bool FlushRule::Test(const TArray<int>& Cards)
{
	int CurrentSuit = GET_SUIT(Cards[0]);
	for (int Card : Cards)
	{
		int Suit = GET_SUIT(Card);
		if (Suit != CurrentSuit)
		{
			return false;
		}
	}
	
	return true;
}

int FlushRule::Compare(const TArray<int>& FirstHand, const TArray<int>& SecondHand)
{
	const TArray<int>& FirstCards = FirstHand;
	const TArray<int>& SecondCards = SecondHand;
	
	for (int Index = 0; Index < FirstCards.Num(); ++Index)
	{
		int FirstRank = GET_RANK(FirstCards[Index]);
		int SecondRank = GET_RANK(SecondCards[Index]);
		if (FirstRank > SecondRank)
		{
			return 1;
		}
		else if (FirstRank < SecondRank)
		{
			return -1;
		}
	}
	
	return 0;
}

bool StraightRule::Test(const TArray<int>& Hand)
{
	TArray<int> Cards = Hand;
	Algo::Sort(Cards);
	for (int Index = 0; Index < Cards.Num() - 1; ++Index)
	{
		int FirstRank = GET_RANK(Cards[Index]);
		int SecondRank = GET_RANK(Cards[Index + 1]);
		if (SecondRank - FirstRank != 1)
		{
			return false;
		}
	}
	
	return true;
}

int StraightRule::Compare(const TArray<int>& FirstHand, const TArray<int>& SecondHand)
{
	int FirstHighestRank = GET_RANK(*Algo::MaxElement(FirstHand));
	int SecondHighestRank = GET_RANK(*Algo::MaxElement(SecondHand));
	
	if (FirstHighestRank > SecondHighestRank)
	{
		return 1;
	}
	else if (FirstHighestRank < SecondHighestRank)
	{
		return -1;
	}
	
	return 0;
}

bool ThreeOfAKindRule::Test(const TArray<int>& Cards)
{
	TMap<int, int> KindCountMap;
	for (int Card : Cards)
	{
		int Kind = GET_RANK(Card);
		if (KindCountMap.Contains(Kind))
		{
			KindCountMap[Kind] += 1;
		}
		else
		{
			KindCountMap[Kind] = 1;
		}
	}
	
	for (const auto& [Kind, Count] : KindCountMap)
	{
		if (Count == 3)
		{
			return true;
		}
	}
	
	return false;
}

int ThreeOfAKindRule::Compare(const TArray<int>& FirstHand, const TArray<int>& SecondHand)
{
	TArray<int> FirstCards = FirstHand;
	TArray<int> SecondCards = SecondHand;
	
	auto Predicate = [](const int A, const int B)
	{
		return A > B;	
	};
	
	Algo::Sort(FirstCards, Predicate);
	Algo::Sort(SecondCards, Predicate);
	
	for (int Index = 0; Index < FirstCards.Num(); ++Index)
	{
		int FirstRank = GET_RANK(FirstCards[Index]);
		int SecondRank = GET_RANK(SecondCards[Index]);
		
		if (FirstRank > SecondRank)
		{
			return 1;
		}
		else if (FirstRank < SecondRank)
		{
			return -1;
		}
	}
	
	return 0;
}

bool TwoPairRule::Test(const TArray<int>& Cards)
{
	TMap<int, int> KindCountMap;
	for (int Card : Cards)
	{
		int Kind = GET_RANK(Card);
		if (KindCountMap.Contains(Kind))
		{
			KindCountMap[Kind] += 1;
		}
		else
		{
			KindCountMap[Kind] = 1;
		}
	}
	
	int NumPairs = 0;
	for (const auto& [Kind, Count] : KindCountMap)
	{
		if (Count == 2)
		{
			++NumPairs;
			continue;
		}
	}
	
	return NumPairs == 2;
}

int TwoPairRule::Compare(const TArray<int>& FirstHand, const TArray<int>& SecondHand)
{
	TArray<int> FirstCards = FirstHand;
	TArray<int> SecondCards = SecondHand;
	
	auto Predicate = [](const int A, const int B)
	{
		return A > B;	
	};
	
	Algo::Sort(FirstCards, Predicate);
	Algo::Sort(SecondCards, Predicate);
	
	for (int Index = 0; Index < FirstCards.Num(); ++Index)
	{
		int FirstRank = GET_RANK(FirstCards[Index]);
		int SecondRank = GET_RANK(SecondCards[Index]);
		
		if (FirstRank > SecondRank)
		{
			return 1;
		}
		else if (FirstRank < SecondRank)
		{
			return -1;
		}
	}
	
	return 0;
}

bool OnePairRule::Test(const TArray<int>& Cards)
{
	TMap<int, int> KindCountMap;
	for (int Card : Cards)
	{
		int Kind = GET_RANK(Card);
		if (KindCountMap.Contains(Kind))
		{
			KindCountMap[Kind] += 1;
		}
		else
		{
			KindCountMap[Kind] = 1;
		}
	}
	
	for (const auto& [Kind, Count] : KindCountMap)
	{
		if (Count == 2)
		{
			return true;
		}
	}
	
	return false;
}

int OnePairRule::Compare(const TArray<int>& FirstHand, const TArray<int>& SecondHand)
{
	TArray<int> FirstCards = FirstHand;
	TArray<int> SecondCards = SecondHand;
	
	auto Predicate = [](const int A, const int B)
	{
		return A > B;	
	};
	
	Algo::Sort(FirstCards, Predicate);
	Algo::Sort(SecondCards, Predicate);
	
	for (int Index = 0; Index < FirstCards.Num(); ++Index)
	{
		int FirstRank = GET_RANK(FirstCards[Index]);
		int SecondRank = GET_RANK(SecondCards[Index]);
		
		if (FirstRank > SecondRank)
		{
			return 1;
		}
		else if (FirstRank < SecondRank)
		{
			return -1;
		}
	}
	
	return 0;
}

bool HighCardRule::Test(const TArray<int>& Hand)
{
	return true;
}

int HighCardRule::Compare(const TArray<int>& FirstHand, const TArray<int>& SecondHand)
{
	TArray<int> FirstCards = FirstHand;
	TArray<int> SecondCards = SecondHand;
	
	auto Predicate = [](const int A, const int B)
	{
		return A > B;	
	};
	
	Algo::Sort(FirstCards, Predicate);
	Algo::Sort(SecondCards, Predicate);
	
	for (int Index = 0; Index < FirstCards.Num(); ++Index)
	{
		int FirstRank = GET_RANK(FirstCards[Index]);
		int SecondRank = GET_RANK(SecondCards[Index]);
		
		if (FirstRank > SecondRank)
		{
			return 1;
		}
		else if (FirstRank < SecondRank)
		{
			return -1;
		}
	}
	
	return 0;
}
