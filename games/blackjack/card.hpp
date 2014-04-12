#include <iostream>

using namespace std;

#ifndef __CARD__
#define __CARD__

class Card {

public: 

  // a card's 5 different state
  // add more players here to extend implementation
  // okay since only a small maximum number of players ever
  enum card_state_enum{
    DECK,
    DEALER_FACE_DOWN,
    DEALER_FACE_UP,
    PLAYER_FACE_UP,
    DISCARD
  };

  Card(char suit_, int level_) {
    if (suit_ != 'S' && suit_ != 'D' && suit_ != 'C'&& suit_ != 'H') {
      cerr << "Invalid suit: "<< suit_ << endl;
      throw("Invalid suit");
    }
    if (level_ < 1 && level > 13) {
      cerr << "Invalid level: "<< level_ << endl;
      throw("Invalid level"); 
    }
    suit = suit_;
    level = level_;
    card_state = DECK;
  }

  void set_card_state(Card::card_state_enum state) {
    card_state = state;
  }

  card_state_enum get_card_state() {
    return card_state;
  }
  
  char get_suit() {
    return suit;
  }

  int get_level() {
    return level;
  }
  
  int get_value() {
    if (level >= 2 && level <= 10)
      return level;
    if (level >= 11 && level <= 13)
      return 10;
    else
      // treat aces as 11 in this function
      return 11; 
  }

  void print() {
    if (level <= 10 && level >= 2)
      cout << level << suit;
    else if (level == 1) 
      cout << 'A' << suit;
    else if (level == 11)
      cout << 'J' << suit;
    else if (level == 12)
      cout << 'Q' << suit;
    else if (level == 13)
      cout << 'K' << suit;
  }

protected:

  // see enum declaration
  card_state_enum card_state;
  
  // 'S': spade, 'D': diamond, 'C': club, 'H': heart
  char suit;

  // 1: ace, 2: 2, ... , 10: 10, 11: Jack, 12: Queen, 13: King
  int level;
};

#endif
