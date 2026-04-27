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
	TArray<int> NewHand;
	ensureAlways(!Cards.IsEmpty());
	
	for (int Index = 0; Index < HAND_SIZE; ++Index)
	{
		NewHand.Push(Cards[Index]);
		Cards.RemoveAtSwap(Index);
	}
	
	Cards.Shrink();
	return NewHand;
}

void URFCards::Reset()
{
	LastHandEndIndex = -1;
	Cards.Init(0, NUM_PLAYING_CARDS);
	for (int Index = 0; Index < NUM_PLAYING_CARDS; ++Index)
	{
		Cards[Index] = Index;
	}
}

void URFCards::ReplaceDiscardedCards(const TArray<int> CardIndicesToBeDiscarded, TArray<int>& TargetHand)
{
	for (int CardIndex : CardIndicesToBeDiscarded)
	{
		if (CardIndex == -1)
		{
			continue;
		}
		
		TargetHand[CardIndex] = Cards[0];
		Cards.RemoveAtSwap(0);
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

void URFPokerStatus::SetStatus(const FString Text)
{
	StatusText = FText::FromString(Text);
}
