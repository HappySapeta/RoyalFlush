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

bool URFRoyalFlushRule::Test(const TArray<int>& Cards) const 
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

EPokerRankComparision URFRoyalFlushRule::Compare(const TArray<int>& FirstHand, const TArray<int>& SecondHand) const 
{
	return EPokerRankComparision::SAME;
}

bool URFStraightFlushRule::Test(const TArray<int>& Cards) const 
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

EPokerRankComparision URFStraightFlushRule::Compare(const TArray<int>& FirstHand, const TArray<int>& SecondHand) const 
{
	TArray<int> FirstCards = FirstHand;
	TArray<int> SecondCards = SecondHand;
	
	auto Predicate = [](const int A, const int B)
	{
		return GET_RANK(A) > GET_RANK(B);
	};
	
	Algo::Sort(FirstCards, Predicate);
	Algo::Sort(SecondCards, Predicate);
	
	if (GET_RANK(FirstCards[0]) > GET_RANK(SecondCards[0]))
	{
		return EPokerRankComparision::HIGHER;
	}
	else if (GET_RANK(FirstCards[0]) < GET_RANK(SecondCards[0]))
	{
		return EPokerRankComparision::LOWER;
	}
	
	return EPokerRankComparision::SAME;
}

bool URFFourOfAKindRule::Test(const TArray<int>& Cards) const 
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
			KindCountMap.Add(Kind, 1);
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

EPokerRankComparision URFFourOfAKindRule::Compare(const TArray<int>& FirstHand, const TArray<int>& SecondHand) const 
{
	TArray<int> FirstCards = FirstHand;
	TArray<int> SecondCards = SecondHand;
	
	auto Predicate = [](const int A, const int B)
	{
		return GET_RANK(A) > GET_RANK(B);
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
			return EPokerRankComparision::HIGHER;
		}
		else if (FirstRank < SecondRank)
		{
			return EPokerRankComparision::LOWER;
		}
	}
	
	return EPokerRankComparision::SAME;
}

bool URFFullHouseRule::Test(const TArray<int>& Cards) const 
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
			KindCountMap.Add(Kind, 1);
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

EPokerRankComparision URFFullHouseRule::Compare(const TArray<int>& FirstHand, const TArray<int>& SecondHand) const 
{
	TArray<int> FirstCards = FirstHand;
	TArray<int> SecondCards = SecondHand;
	
	auto Predicate = [](const int A, const int B)
	{
		return GET_RANK(A) > GET_RANK(B);
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
			return EPokerRankComparision::HIGHER;
		}
		else if (FirstRank < SecondRank)
		{
			return EPokerRankComparision::LOWER;
		}
	}
	
	return EPokerRankComparision::SAME;
}

bool URFFlushRule::Test(const TArray<int>& Cards) const 
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

EPokerRankComparision URFFlushRule::Compare(const TArray<int>& FirstHand, const TArray<int>& SecondHand) const 
{
	TArray<int> FirstCards = FirstHand;
	TArray<int> SecondCards = SecondHand;
	
	auto Predicate = [](const int A, const int B)
	{
		return GET_RANK(A) > GET_RANK(B);	
	};
	
	Algo::Sort(FirstCards, Predicate);
	Algo::Sort(SecondCards, Predicate);
	
	for (int Index = 0; Index < FirstCards.Num(); ++Index)
	{
		int FirstRank = GET_RANK(FirstCards[Index]);
		int SecondRank = GET_RANK(SecondCards[Index]);
		
		if (FirstRank > SecondRank)
		{
			return EPokerRankComparision::HIGHER;
		}
		else if (FirstRank < SecondRank)
		{
			return EPokerRankComparision::LOWER;
		}
	}
	
	return EPokerRankComparision::SAME;
}

bool URFStraightRule::Test(const TArray<int>& Hand) const 
{
	TArray<int> Cards = Hand;
	
	auto Predicate = [](const int A, const int B)
	{
		return GET_RANK(A) > GET_RANK(B);	
	};
	
	Algo::Sort(Cards, Predicate);
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

EPokerRankComparision URFStraightRule::Compare(const TArray<int>& FirstHand, const TArray<int>& SecondHand) const 
{
	int FirstHighestRank = GET_RANK(*Algo::MaxElement(FirstHand));
	int SecondHighestRank = GET_RANK(*Algo::MaxElement(SecondHand));
	
	if (FirstHighestRank > SecondHighestRank)
	{
		return EPokerRankComparision::HIGHER;
	}
	else if (FirstHighestRank < SecondHighestRank)
	{
		return EPokerRankComparision::LOWER;
	}
	
	return EPokerRankComparision::SAME;
}

bool URFThreeOfAKindRule::Test(const TArray<int>& Cards) const 
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
			KindCountMap.Add(Kind, 1);
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

EPokerRankComparision URFThreeOfAKindRule::Compare(const TArray<int>& FirstHand, const TArray<int>& SecondHand) const 
{
	TArray<int> FirstCards = FirstHand;
	TArray<int> SecondCards = SecondHand;
	
	auto Predicate = [](const int A, const int B)
	{
		return GET_RANK(A) > GET_RANK(B);	
	};
	
	Algo::Sort(FirstCards, Predicate);
	Algo::Sort(SecondCards, Predicate);
	
	for (int Index = 0; Index < FirstCards.Num(); ++Index)
	{
		int FirstRank = GET_RANK(FirstCards[Index]);
		int SecondRank = GET_RANK(SecondCards[Index]);
		
		if (FirstRank > SecondRank)
		{
			return EPokerRankComparision::HIGHER;
		}
		else if (FirstRank < SecondRank)
		{
			return EPokerRankComparision::LOWER;
		}
	}
	
	return EPokerRankComparision::SAME;
}

bool URFTwoPairRule::Test(const TArray<int>& Cards) const 
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
			KindCountMap.Add(Kind, 1);
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

EPokerRankComparision URFTwoPairRule::Compare(const TArray<int>& FirstHand, const TArray<int>& SecondHand) const 
{
	TArray<int> FirstCards = FirstHand;
	TArray<int> SecondCards = SecondHand;
	
	auto Predicate = [](const int A, const int B)
	{
		return GET_RANK(A) > GET_RANK(B);	
	};
	
	Algo::Sort(FirstCards, Predicate);
	Algo::Sort(SecondCards, Predicate);
	
	for (int Index = 0; Index < FirstCards.Num(); ++Index)
	{
		int FirstRank = GET_RANK(FirstCards[Index]);
		int SecondRank = GET_RANK(SecondCards[Index]);
		
		if (FirstRank > SecondRank)
		{
			return EPokerRankComparision::HIGHER;
		}
		else if (FirstRank < SecondRank)
		{
			return EPokerRankComparision::LOWER;
		}
	}
	
	return EPokerRankComparision::SAME;
}

bool URFOnePairRule::Test(const TArray<int>& Cards) const 
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
			KindCountMap.Add(Kind, 1);
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

EPokerRankComparision URFOnePairRule::Compare(const TArray<int>& FirstHand, const TArray<int>& SecondHand) const 
{
	TArray<int> FirstCards = FirstHand;
	TArray<int> SecondCards = SecondHand;
	
	auto Predicate = [](const int A, const int B)
	{
		return GET_RANK(A) > GET_RANK(B);
	};
	
	Algo::Sort(FirstCards, Predicate);
	Algo::Sort(SecondCards, Predicate);
	
	for (int Index = 0; Index < FirstCards.Num(); ++Index)
	{
		int FirstRank = GET_RANK(FirstCards[Index]);
		int SecondRank = GET_RANK(SecondCards[Index]);
		
		if (FirstRank > SecondRank)
		{
			return EPokerRankComparision::HIGHER;
		}
		else if (FirstRank < SecondRank)
		{
			return EPokerRankComparision::LOWER;
		}
	}
	
	return EPokerRankComparision::SAME;
}

bool URFHighCardRule::Test(const TArray<int>& Hand) const 
{
	return true;
}

EPokerRankComparision URFHighCardRule::Compare(const TArray<int>& FirstHand, const TArray<int>& SecondHand) const 
{
	TArray<int> FirstCards = FirstHand;
	TArray<int> SecondCards = SecondHand;
	
	auto Predicate = [](const int A, const int B)
	{
		return GET_RANK(A) > GET_RANK(B);	
	};
	
	Algo::Sort(FirstCards, Predicate);
	Algo::Sort(SecondCards, Predicate);
	
	for (int Index = 0; Index < FirstCards.Num(); ++Index)
	{
		int FirstRank = GET_RANK(FirstCards[Index]);
		int SecondRank = GET_RANK(SecondCards[Index]);
		
		if (FirstRank > SecondRank)
		{
			return EPokerRankComparision::HIGHER;
		}
		else if (FirstRank < SecondRank)
		{
			return EPokerRankComparision::LOWER;
		}
	}
	
	return EPokerRankComparision::SAME;
}
