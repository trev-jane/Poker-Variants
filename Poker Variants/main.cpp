#include <algorithm>
#include <cstdlib> // rand(), srand()
#include <ctime> // time()
#include <iostream>
#include <iomanip>
#include <vector>

struct Card;

using Suit = int;
using Hand = std::vector<Card>;
using Deck = std::vector<Card>;
static Suit const HEARTS = 0;
static Suit const DIAMONDS = 1;
static Suit const CLUBS = 2;
static Suit const SPADES = 3;
static Suit const STARS = 4;
static uint32_t const TRIALS = 2 << 18;

struct Card
{
  Card(int rank, Suit suit) : rank(rank), suit(suit) {}
  int rank;
  Suit suit;
};

bool operator<(Card const& lhs, Card const& rhs)
{
  return lhs.rank < rhs.rank;
}

std::vector<Card> BuildDeck(int ranks = 13, int suits = 4)
{
  Deck deck;

  for (int i = 0; i < suits; ++i)
  {
    for (int j = 2; j <= ranks + 1; ++j)
    {
      deck.emplace_back(j, i);
    }
  }

  return deck;
}

std::vector<Card> DealHand(int size = 5)
{
  Hand hand;
  Deck deck = BuildDeck();

  for (int i = 0; i < size; ++i)
  {
    int card = rand() % static_cast<int>(deck.size());
    hand.push_back(deck[card]);
    deck[card] = deck[deck.size() - 1];
    deck.pop_back();
  }

  std::sort(hand.begin(), hand.end());
  return hand;
}

bool IsFourOfAKind(Hand const& hand)
{
  if (hand.size() < 4)
  {
    return false;
  }

  for (size_t i = 0; i < hand.size() - 3; ++i)
  {
    if (hand[i].rank == hand[i + 1].rank && hand[i].rank == hand[i + 3].rank)
    {
      return true;
    }
  }

  return false;
}

bool IsFullHouse(Hand const& hand)
{
  if (hand.size() < 5)
  {
    return false;
  }

  size_t current = 0;

  while (hand[current].rank != hand[current + 1].rank)
  {
    if (++current > hand.size() - 5)
    {
      return false;
    }
  }

  bool foundPair = hand[current].rank != hand[current + 2].rank;
  current += 3 - static_cast<size_t>(foundPair);

  while (hand[current].rank != hand[current + 1].rank)
  {
    if (++current > hand.size() - (2 + static_cast<size_t>(foundPair)))
    {
      return false;
    }
  }

  return hand[current].rank == hand[current + 1 + static_cast<size_t>(foundPair)].rank;
}

bool IsFullerHouse(Hand const& hand)
{
  if (hand.size() != 6)
  {
    return false;
  }

  if (hand[0].rank == hand[1].rank && hand[4].rank == hand[5].rank)
  {
    return hand[2].rank == hand[5].rank || hand[3].rank == hand[0].rank;
  }

  return false;
}

bool IsDoubleSet(Hand const& hand)
{
  if (hand.size() != 6)
  {
    return false;
  }

  return hand[0].rank == hand[2].rank && hand[3].rank == hand[5].rank;
}

bool IsFlush(Hand const& hand)
{
  for (size_t i = 1; i < hand.size(); ++i)
  {
    if (hand[i - 1].suit != hand[i].suit)
    {
      return false;
    }
  }

  return true;
}

bool IsStraight(Hand const& hand)
{
  for (size_t i = 1; i < hand.size(); ++i)
  {
    if (hand[i - 1].rank != hand[i].rank - 1)
    {
      return false;
    }
  }

  return true;
}

bool IsThreePair(Hand const& hand)
{
  if (hand.size() < 6)
  {
    return false;
  }

  return hand[0].rank == hand[1].rank && hand[2].rank == hand[3].rank && hand[4].rank == hand[5].rank
    && hand[0].rank != hand[2].rank && hand[2].rank != hand[4].rank;
}

bool IsThreeOfAKind(Hand const& hand)
{
  if (hand.size() < 3)
  {
    return false;
  }

  for (size_t i = 0; i < hand.size() - 2; ++i)
  {
    if (hand[i].rank == hand[i + 2].rank)
    {
      return true;
    }

    return false;
  }
}

bool IsTwoPair(Hand const& hand)
{
  if (hand.size() < 4)
  {
    return false;
  }

  for (size_t i = 0; i < hand.size() - 3; ++i)
  {
    if (hand[i].rank == hand[i + 1].rank)
    {
      for (size_t j = i + 2; j < hand.size() - 1; ++j)
      {
        if (hand[j].rank == hand[j + 1].rank)
        {
          return true;
        }
      }
    }
  }

  return false;
}

bool IsPair(Hand const& hand)
{
  for (size_t i = 0; i < hand.size() - 1; ++i)
  {
    if (hand[i].rank == hand[i + 1].rank)
    {
      return true;
    }
  }

  return false;
}



void SixCardRankings()
{
  int straightFlushes = 0,
    fourOfAKinds = 0,
    fullerHouses = 0,
    doubleSets = 0,
    fullHouses = 0,
    flushes = 0,
    straights = 0,
    threePairs = 0,
    threeOfAKinds = 0,
    twoPairs = 0,
    pairs = 0;

  Deck deck = BuildDeck();

  for (size_t i = 0; i < deck.size(); ++i)
  {
    for (size_t j = i + 1; j < deck.size(); ++j)
    {
      for (size_t k = j + 1; k < deck.size(); ++k)
      {
        for (size_t l = k + 1; l < deck.size(); ++l)
        {
          for (size_t m = l + 1; m < deck.size(); ++m)
          {
            for (size_t n = m + 1; n < deck.size(); ++n)
            {
              Hand hand;
              hand.push_back(deck[i]);
              hand.push_back(deck[j]);
              hand.push_back(deck[k]);
              hand.push_back(deck[l]);
              hand.push_back(deck[m]);
              hand.push_back(deck[n]);
              std::sort(hand.begin(), hand.end());

              if (IsStraight(hand))
              {
                if (!IsFlush(hand))
                {
                  ++straights;
                }

                else
                {
                  ++straightFlushes;
                }
              }

              else if (IsDoubleSet(hand))
              {
                ++doubleSets;
              }

              else if (IsFullerHouse(hand))
              {
                ++fullerHouses;
              }

              else if (IsFullHouse(hand))
              {
                ++fullHouses;
              }

              else if (IsThreePair(hand))
              {
                ++threePairs;
              }

              else if (IsFourOfAKind(hand))
              {
                ++fourOfAKinds;
              }

              else if (IsFlush(hand))
              {
                ++flushes;
              }

              else if (IsThreeOfAKind(hand))
              {
                ++threeOfAKinds;
              }

              else if (IsTwoPair(hand))
              {
                ++twoPairs;
              }

              else if (IsPair(hand))
              {
                ++pairs;
              }
            }
          }
        }
      }
    }
  }

  std::cout << "Straight Flushes: " << straightFlushes << std::endl;
  std::cout << "Fuller Houses:    " << fullerHouses << std::endl;
  std::cout << "Double Sets:      " << doubleSets << std::endl;
  std::cout << "Flushes:          " << flushes << std::endl;
  std::cout << "Four-of-a-Kinds:  " << fourOfAKinds << std::endl;
  std::cout << "Straights:        " << straights << std::endl;
  std::cout << "Three Pairs:      " << threePairs << std::endl;
  std::cout << "Full Houses:      " << fullHouses << std::endl;
  std::cout << "Three-of-a-Kinds: " << threeOfAKinds << std::endl;
  std::cout << "Two Pairs:        " << twoPairs << std::endl;
  std::cout << "Pairs:            " << pairs << std::endl;
}

void FourCardRankings()
{
  int straightFlushes = 0,
    fourOfAKinds = 0,
    flushes = 0,
    straights = 0,
    threeOfAKinds = 0,
    twoPairs = 0,
    pairs = 0;

  Deck deck = BuildDeck();

  for (size_t i = 0; i < deck.size(); ++i)
  {
    for (size_t j = i + 1; j < deck.size(); ++j)
    {
      for (size_t k = j + 1; k < deck.size(); ++k)
      {
        for (size_t l = k + 1; l < deck.size(); ++l)
        {
          Hand hand;
          hand.push_back(deck[i]);
          hand.push_back(deck[j]);
          hand.push_back(deck[k]);
          hand.push_back(deck[l]);
          std::sort(hand.begin(), hand.end());

          if (IsStraight(hand))
          {
            if (!IsFlush(hand))
            {
              ++straights;
            }

            else
            {
              ++straightFlushes;
            }
          }

          else if (IsFourOfAKind(hand))
          {
            ++fourOfAKinds;
          }

          else if (IsFlush(hand))
          {
            ++flushes;
          }

          else if (IsThreeOfAKind(hand))
          {
            ++threeOfAKinds;
          }

          else if (IsTwoPair(hand))
          {
            ++twoPairs;
          }

          else if (IsPair(hand))
          {
            ++pairs;
          }
        }
      }
    }
  }

  (straights *= 11) /= 10; // Account for A-2-3-4
  std::cout << "Four-of-a-Kinds:  " << fourOfAKinds << std::endl;
  std::cout << "Straight Flushes: " << straightFlushes << std::endl;
  std::cout << "Three-of-a-Kinds: " << threeOfAKinds << std::endl;
  std::cout << "Straights:        " << straights << std::endl;
  std::cout << "Two Pairs:        " << twoPairs << std::endl;
  std::cout << "Flushes:          " << flushes << std::endl;
  std::cout << "Pairs:            " << pairs << std::endl;
}

int main()
{
  std::srand(static_cast<unsigned>(std::time(0)));
  char const* s = R"(        -POKER HAND-SIZE VARIANTS-

This program determines the hand rankings
for variations of poker with four-card hands
and six-card hands instead of the standard
five. It deals all possible six-card and
four-card combinations and counts the
frequencies of each hand.

Four-card poker does not have a full house.

Six-card poker has three additional hands:
  *Fuller House : Four-of-a-Kind + Pair
  *Double Set   : Two Three-of-a-Kinds
  *Three Pair   : Three Pairs


)";

  std::cout << s << "Four-Card Poker Hand Counts:\n\n";
  FourCardRankings();
  std::cout << "\n\n\nSix-Card Poker Hand Counts (takes a while):\n\n";
  SixCardRankings();
  std::cout << "\n\n";
  return 0;
}