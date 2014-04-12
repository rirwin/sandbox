#include <iostream>
#include "card_deck.hpp"
#include "card.hpp"
#include "player.hpp"

using namespace std;

#ifndef __GAME__
#define __GAME__

class Game {

public:

  Game(int num_decks) {
    card_deck = new CardDeck(num_decks);
    player = new Player(false);
    dealer = new Player(true);
  }

  ~Game() {
    delete card_deck;
    delete player;
    delete dealer;
  }
  
  void play_new_hand() {
    deal_initial_cards();
    // check blackjack
    player->take_turn();
    dealer->take_turn();
  }

protected:

  Player *player;
  Player *dealer;
  CardDeck *card_deck;

  void deal_initial_cards() {
    
    player->receive_card(card_deck->deal_card(Card::DEALER_FACE_UP));
    player->receive_card(card_deck->deal_card(Card::DEALER_FACE_UP));

    dealer->receive_card(card_deck->deal_card(Card::DEALER_FACE_DOWN));
    dealer->receive_card(card_deck->deal_card(Card::DEALER_FACE_UP));
  }
  


};

#endif
