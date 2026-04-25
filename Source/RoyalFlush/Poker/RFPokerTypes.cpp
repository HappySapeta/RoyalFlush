#include "RFPokerTypes.h"
#include "RFPokerDeck.h"
#include "Algo/RandomShuffle.h"

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
