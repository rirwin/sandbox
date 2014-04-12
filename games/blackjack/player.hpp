#include <iostream>

using namespace std;

#ifndef __PLAYER__
#define __PLAYER__

class Player{

public:
  Player(bool is_dealer_) {
    is_dealer = is_dealer_;
  }
  ~Player() {
  }

  void receive_card(Card* card) {
    cards.push_back(card);
  }
  
  
protected:
  vector<Card*> cards;
  int chips;
  bool is_dealer;
};

#endif
