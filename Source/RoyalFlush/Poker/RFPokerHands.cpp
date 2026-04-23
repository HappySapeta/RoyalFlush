#include "RFPokerHands.h"

#include "RFStandingCards.h"
#include "Algo/RandomShuffle.h"
#include "Chaos/ConvexFlattenedArrayStructureData.h"

constexpr int GET_RANK(const int Card)
{
	return Card % 13;
}
	
constexpr int GET_SUIT(const int Card)
{
	return Card / 13;
}

URFCards::URFCards()
{
	for (int Index = 0; Index < NUM_PLAYING_CARDS; ++Index)
	{
		Cards.Push(Index);
	}
}

void URFCards::Shuffle()
{
	Algo::RandomShuffle(Cards);
}

TArray<int> URFCards::NewHand()
{
	int NextHandStartIndex = LastHandEndIndex + 1;
	if (!ensureAlways(Cards.IsValidIndex(NextHandStartIndex) && Cards.IsValidIndex(NextHandStartIndex + (HAND_SIZE - 1))))
	{
		return {};
	}
	
	int* Start = &Cards[NextHandStartIndex];
	LastHandEndIndex = NextHandStartIndex + (HAND_SIZE - 1);
	
	TArray<int>NewHand{Start, HAND_SIZE};
	
	for (int Index = NextHandStartIndex; Index <= LastHandEndIndex; ++Index)
	{
		Cards.RemoveAtSwap(Index, 1, EAllowShrinking::No);
	}
	
	Cards.Shrink();
	
	return NewHand;
}

void URFCards::Reset()
{
	LastHandEndIndex = -1;
	for (int Index = 0; Index < NUM_PLAYING_CARDS; ++Index)
	{
		Cards.Push(Index);
	}
}

int URFCards::SwapCard(int Card)
{
	int NewCard = Cards[0];
	Cards.RemoveAtSwap(0);
	Cards.Push(Card);
	return NewCard;
}

void URFCards::ReplaceDiscardedCards(const TArray<int> CardIndicesToBeDiscarded, TArray<int>& TargetHand)
{
	for (int CardIndex : CardIndicesToBeDiscarded)
	{
		if (CardIndex == -1)
		{
			continue;
		}
		
		TargetHand[CardIndex] = SwapCard(TargetHand[CardIndex]); 
	}
	
	Cards.Shrink();
}

const TArray<int>& URFHand::GetCards() const
{
	return Cards;
}

void URFHand::SetCards(const TArray<int>& NewCards)
{
	ensure(NewCards.Num() == HAND_SIZE);
	Cards = NewCards;
}

bool URFHand::Equals(const URFHand* Other) const
{
	for (int Index = 0; Index < HAND_SIZE; ++Index)
	{
		if (Cards[Index] != Other->Cards[Index])
		{
			return false;
		}
	}
	
	return true;
}

int URFRankedHands::GetRank(URFHand* Hand)
{
	int RankIndex = RankedHands.Num();
	for (int Index = 0; Index < RankedHands.Num(); ++Index)
	{
		if (Hand->Equals(RankedHands[Index]))
		{
			RankIndex = Index;
			break;
		}
	}
	
	return RankIndex;
}

bool URFRankedHands::IsFirstHigherThanSecond(URFHand* First, URFHand* Second)
{
	int FirstRank = GetRank(First);
	int SecondRank = GetRank(Second);
	return FirstRank < SecondRank;
}

bool RoyalFlushRule::Test(const URFHand* Hand)
{
	const TArray<int> Cards = Hand->GetCards();
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

int RoyalFlushRule::Compare(const URFHand* FirstHand, const URFHand* SecondHand)
{
	return 0;
}

bool StraightFlushRule::Test(const URFHand* Hand)
{
	const TArray<int> Cards = Hand->GetCards();
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

int StraightFlushRule::Compare(const URFHand* FirstHand, const URFHand* SecondHand)
{
	auto SumOperation = [](const int A, const int B){ return A + B; };
	int FirstSum = Algo::Accumulate(FirstHand->GetCards(), 0, SumOperation);
	int SecondSum = Algo::Accumulate(FirstHand->GetCards(), 0, SumOperation);
	
	return FirstSum > SecondSum;
}

bool FourOfAKindRule::Test(const URFHand* Hand)
{
	TMap<int, int> KindCountMap;
	for (int Card : Hand->GetCards())
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

int FourOfAKindRule::Compare(const URFHand* FirstHand, const URFHand* SecondHand)
{
	TArray<int> FirstCards = FirstHand->GetCards();
	TArray<int> SecondCards = SecondHand->GetCards();
	
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

bool FullHouseRule::Test(const URFHand* Hand)
{
	TMap<int, int> KindCountMap;
	for (int Card : Hand->GetCards())
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

int FullHouseRule::Compare(const URFHand* FirstHand, const URFHand* SecondHand)
{
	TArray<int> FirstCards = FirstHand->GetCards();
	TArray<int> SecondCards = SecondHand->GetCards();
	
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

bool FlushRule::Test(const URFHand* Hand)
{
	int CurrentSuit = GET_SUIT(Hand->GetCards()[0]);
	for (int Card : Hand->GetCards())
	{
		int Suit = GET_SUIT(Card);
		if (Suit != CurrentSuit)
		{
			return false;
		}
	}
	
	return true;
}

int FlushRule::Compare(const URFHand* FirstHand, const URFHand* SecondHand)
{
	const TArray<int>& FirstCards = FirstHand->GetCards();
	const TArray<int>& SecondCards = SecondHand->GetCards();
	
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

bool StraightRule::Test(const URFHand* Hand)
{
	TArray<int> Cards = Hand->GetCards();
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

int StraightRule::Compare(const URFHand* FirstHand, const URFHand* SecondHand)
{
	int FirstHighestRank = GET_RANK(*Algo::MaxElement(FirstHand->GetCards()));
	int SecondHighestRank = GET_RANK(*Algo::MaxElement(SecondHand->GetCards()));
	
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

bool ThreeOfAKindRule::Test(const URFHand* Hand)
{
	TMap<int, int> KindCountMap;
	for (int Card : Hand->GetCards())
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

int ThreeOfAKindRule::Compare(const URFHand* FirstHand, const URFHand* SecondHand)
{
	TArray<int> FirstCards = FirstHand->GetCards();
	TArray<int> SecondCards = SecondHand->GetCards();
	
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

bool TwoPairRule::Test(const URFHand* Hand)
{
	TMap<int, int> KindCountMap;
	for (int Card : Hand->GetCards())
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

int TwoPairRule::Compare(const URFHand* FirstHand, const URFHand* SecondHand)
{
	TArray<int> FirstCards = FirstHand->GetCards();
	TArray<int> SecondCards = SecondHand->GetCards();
	
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

bool OnePairRule::Test(const URFHand* Hand)
{
	TMap<int, int> KindCountMap;
	for (int Card : Hand->GetCards())
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

int OnePairRule::Compare(const URFHand* FirstHand, const URFHand* SecondHand)
{
	TArray<int> FirstCards = FirstHand->GetCards();
	TArray<int> SecondCards = SecondHand->GetCards();
	
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

bool HighCardRule::Test(const URFHand* Hand)
{
	return true;
}

int HighCardRule::Compare(const URFHand* FirstHand, const URFHand* SecondHand)
{
	TArray<int> FirstCards = FirstHand->GetCards();
	TArray<int> SecondCards = SecondHand->GetCards();
	
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
