#ifndef __CARD_DECK__
#define __CARD_DECK__

#include <iostream>
#include <time.h>
#include <vector>
#include <stdlib.h>
#include "card.hpp"

using namespace std;

class CardDeck {

public:

  CardDeck(int num_decks_) {
    num_decks = num_decks_;
    add_cards(num_decks);
    srand(time(NULL));
    shuffle();
    next_card_idx = 0;
  }

  Card* deal_card(Card::card_state_enum state) {
    if (next_card_idx == cards.size())
      return NULL;
    Card* next_card = &cards[next_card_idx];
    next_card->set_state(state);
    next_card_idx++;
    return next_card;
  }

  int get_num_cards_left() {
    return (cards.size() - next_card_idx);
  }

  void shuffle() {
    int num_cards = cards.size();
    for (int i = 0; i < num_cards; i++){
      int j = rand() % num_cards;
      Card tmp = cards[i];
      cards[i] = cards[j];
      cards[j] = tmp;
      cards[i].set_state(Card::DECK);
    }
    next_card_idx = 0;
  }


protected:

  // collection of cards
  vector<Card> cards;

  // number of 52-card decks
  int num_decks;

  // next card to be dealt
  int next_card_idx;

  void add_cards(int num_decks) {
    for (int deck_i = 0; deck_i < num_decks; deck_i++) {
      add_suit_of_deck('S');
      add_suit_of_deck('D');
      add_suit_of_deck('C');
      add_suit_of_deck('H');
    }
  }

  void add_suit_of_deck(char s) {
    for (int i = 1; i < 14; i++) {
      try {
	cards.push_back(Card(s,i));
      }
      catch (char const *err_str) {
	cout << *err_str << endl;
      }
    }
  }

  void print() {
    cout << " Printing cards " << endl;
    for (int i = 0; i < cards.size(); i++) {
      cards[i].print();
      cout << " ";
    }
    cout << " Done printing cards " << endl;
  }

};

#endif
