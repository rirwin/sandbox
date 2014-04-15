#ifndef __PLAYER__
#define __PLAYER__

#include <iostream>
#include <stdio.h>
#include "input_helper.hpp"

using namespace std;

class Player{

public:
  enum player_type_enum{
    PLAYER,
    DEALER
  };

  Player(player_type_enum player_type_) {

    player_type = player_type_;
    if (player_type == Player::PLAYER) 
      chips = 100;
    else
      chips = 0;
  }

  ~Player() {
  }

  void receive_card(Card* card) {
    cards.push_back(card);
  }

  int get_chip_count() {
    return chips;
  }
  
  int place_bet(){

    char buff[100];
    sprintf(buff, "You have chips %d. What is your bet?\nEnter an integer between 1 and %d.", chips, chips);
    string prompt(buff);

    int bet = get_int_from_user(prompt, 1, chips);

    chips -= bet;
    return bet;
  }

  void receive_chips(int chips_to_receive) {
    chips += chips_to_receive;
  }

  bool has_blackjack() {
    if ((cards.size() == 2) && (cards[0]->get_value() + cards[1]->get_value() == 21))
      return true;
    return false;
  }

  void print_cards() {
    for (int i = 0; i < cards.size(); i++) {
      if (cards[i]->get_state() == Card::DEALER_FACE_DOWN)
	cout << "[]";
      else
	cards[i]->print();
      cout << " ";
    }
    cout << endl;
  }

  int get_hand_value() {
    int sum = 0;
    for (int i = 0; i < cards.size(); i++) 
      sum += cards[i]->get_value();

    int i = 0;
    while (sum > 21 && i < cards.size()) {
      if (cards[i]->get_level() == 1)
	sum -= 10;
      i++;
    }
    return sum;
  }
  
  void flip_hole_card() {
    if (player_type == Player::PLAYER) {
      cout << "Error: The player has no hole card" << endl;
    }
    else if (player_type == Player::DEALER) {
      cards[0]->set_state(Card::DEALER_FACE_UP);
    }
  }

  void discard_cards() {
    for (int i = 0; i < cards.size(); i++) 
      cards[i]->set_state(Card::DISCARD);
    cards.clear();
  }

  int get_num_cards() {
    return cards.size();
  }


protected:

  player_type_enum player_type;
  vector<Card*> cards;
  int chips;


};

#endif
