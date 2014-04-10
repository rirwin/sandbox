#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include "card.hpp"

using namespace std;

#ifndef __GAME__
#define __GAME__

class Game {

public:

  Game(int num_decks_) {
    num_decks = num_decks_;
    //srand(time(NULL));

    init_dealer_deck();
    print();
  }

  Card deal_card() {
    if (undealt_cards.size() > 0) {
      Card c = undealt_cards.back();
      undealt_cards.pop_back();
      return c;
    }
    else {
      cout << "No cards left to deal" << endl;
      return NULL;
    }
  }

protected:

  void init_dealer_deck() {
    for (int deck_i = 0; deck_i < num_decks; deck_i++) {
      add_suit_of_deck('S');
      add_suit_of_deck('D');
      add_suit_of_deck('C');
      add_suit_of_deck('H');
    }

    shuffle_deck();
  }
  
  int num_decks;

  // Internally these are vectors for easy of manipulation
  vector<Card> undealt_cards;
  vector<Card> discard_pile;
  vector<Card> dealer;

  // could extend to vector of players
  vector<Card> player_one; 

  void add_suit_of_deck(char s) {
    for (int i = 1; i < 14; i++) {
      undealt_cards.push_back(Card(s,i));
    }
  }

  void print() {
    cout << " Printing cards " << endl;
    for (int i = 0; i < undealt_cards.size(); i++)
      undealt_cards[i].print();
    cout << " Done printing cards " << endl;

  }

  void shuffle_deck() {
    shuffle_deck_once();
  }

  void shuffle_deck_once() {

    int num_cards = undealt_cards.size();

    for (int i = 0; i < num_cards; i++){
      int j = rand() % num_cards;
      Card tmp = undealt_cards[i];
      undealt_cards[i] = undealt_cards[j];
    }
  }

};

#endif
