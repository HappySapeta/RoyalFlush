#include "RFPokerTypes.h"
#include "RFPokerDeck.h"
#include "RFPokerHandStruct.h"
#include "Algo/RandomShuffle.h"
#include "Kismet/KismetMathLibrary.h"

constexpr int NUM_TRIALS = 5;

URFCards::URFCards()
{
	for (int Index = 0; Index < NUM_PLAYING_CARDS; ++Index)
	{
		Cards.Push(Index);
	}
}

void URFCards::SetSpawnData(const UDataTable* Data)
{
	DataTable = Data;
	
	TArray<FRFPokerHandStruct*> Rows;
	Data->GetAllRows(TEXT(""), Rows);
	
	for (const FRFPokerHandStruct* Row : Rows)
	{
		PlayerChances.Add(TRange<float>(PlayerChanceUpperLimit, PlayerChanceUpperLimit + Row->PlayerChance));
		NPCChances.Add(TRange<float>(NPCChanceUpperLimit, NPCChanceUpperLimit + Row->NPCChance));
	
		PlayerChanceUpperLimit += Row->PlayerChance;
		NPCChanceUpperLimit += Row->NPCChance;
	}
}

void URFCards::Shuffle()
{
	Algo::RandomShuffle(Cards);
	DebugLogCards();
}

int URFCards::DrawCard()
{
	int NewCard = Cards.Pop(EAllowShrinking::Yes);
	DebugLogCards();
	return NewCard;
}

int URFCards::DrawCard(const int SpecificCard)
{
	ensureAlways(Cards.Contains(SpecificCard));
	Cards.Remove(SpecificCard);
	Cards.Shrink();
	return SpecificCard;
}

bool URFCards::IsHandDrawable(const TArray<int>& Hand)
{
	for (int Card : Hand)
	{
		if (!Cards.Contains(Card))
		{
			return false;
		}
	}
	
	return true;
}

void URFCards::DrawHand(const TArray<int>& Hand)
{
	for (int Card : Hand)
	{
		DrawCard(Card);
	}
	
	DebugLogCards();
}

TArray<int> URFCards::NewHand(const EPokerPlayer Player)
{
	TArray<TRange<float>>* Chances;
	float ChanceUpperLimit;
	switch (Player)
	{
		case EPokerPlayer::Human:
		{
			Chances = &PlayerChances;
			ChanceUpperLimit = PlayerChanceUpperLimit;
			break;
		}
		case EPokerPlayer::NPC:
		default:
		{
			Chances = &NPCChances;
			ChanceUpperLimit = NPCChanceUpperLimit;
			break;
		}
	}
	
	check(Chances);
	
	float RandomFloat = UKismetMathLibrary::RandomFloatInRange(0.0f, ChanceUpperLimit);
	TArray<FRFPokerHandStruct*> Rows;
	DataTable->GetAllRows(TEXT(""), Rows);
	int TrialsLeft = NUM_TRIALS;
	while (TrialsLeft > 0)
	{
		for (int Index = 0; Index < Rows.Num(); ++Index)
		{
			const FRFPokerHandStruct* Row = Rows[Index];
			if (Chances->operator[](Index).Contains(RandomFloat))
			{
				const TArray<int>& PotentialCards = Row->Cards;
				if (!IsHandDrawable(PotentialCards))
				{
					continue;
				}
				DrawHand(PotentialCards);
				return PotentialCards;
			} 
		}
		--TrialsLeft;
	}
	
	TArray<int> DefaultHand;
	for (int Index = 0; Index < HAND_SIZE; ++Index)
	{
		DefaultHand.Push(DrawCard());
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Creating default hand."));
	return DefaultHand;
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

void URFCards::DebugLogCards()
{
	//FString CardNumbers;
	//for (int Card : Cards)
	//{
	//	CardNumbers += FString::FromInt(Card);
	//	CardNumbers += ", ";
	//}
	//
	//UE_LOG(LogTemp, Warning, TEXT("Cards : %s"), *CardNumbers);
}

void URFCards::ReplaceDiscardedCards(const TArray<int> CardIndicesToBeDiscarded, TArray<int>& TargetHand)
{
	for (int CardIndex : CardIndicesToBeDiscarded)
	{
		if (CardIndex == -1)
		{
			continue;
		}
		
		TargetHand[CardIndex] = DrawCard();
	}
	
	DebugLogCards();
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
