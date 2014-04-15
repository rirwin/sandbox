#include <iostream>
#include <assert.h>


#include "../card.hpp"
#include "../card_deck.hpp"
#include "../player.hpp"
#include "../game.hpp"

using namespace std;

// class used for testing of state of protected variables of
// inherited class Game (no need for friend class)
class GameTester : public Game {

public:
  GameTester(int num_decks, int shoe): Game(num_decks, shoe) {
  }

  int test_hand_result() {

    // already tested get hand value
    // so simulate numbers to avoid iterating through all possible
    // hands for both players
    for (int player_hand_value = 4; player_hand_value < 24; player_hand_value++) {
      for (int dealer_hand_value = 4; dealer_hand_value < 24; dealer_hand_value++) { 

	if (player_hand_value > dealer_hand_value || dealer_hand_value > 21) 
	  assert(get_result(player_hand_value, dealer_hand_value) == Game::PLAYER_WINS);
	else if (dealer_hand_value > player_hand_value || player_hand_value > 21) 
	  assert(get_result(player_hand_value, dealer_hand_value) == Game::DEALER_WINS);
	else if (player_hand_value == dealer_hand_value)
	  assert(get_result(player_hand_value, dealer_hand_value) == Game::PUSH);
      }
    }
    return 0;
  }

  int test_dealer_logic() {
    
    for (int test_i = 0; test_i < 1000; test_i++) {
      if (card_deck->get_num_cards_left() < shuffle_shoe) {
	card_deck->shuffle();
      }
      deal_initial_cards();
      dealer_turn();
      assert(dealer->get_hand_value() >= 17);
      dealer->discard_cards();
    }

    return 0;
  }

  int test_deal_discard_shuffle() {
    
    for (int test_i = 0; test_i < 1000; test_i++) {
      if (card_deck->get_num_cards_left() < shuffle_shoe) {
	card_deck->shuffle();
      }
      deal_initial_cards(); // 2 cards each to player and dealer
      player->receive_card(card_deck->deal_card(Card::PLAYER_FACE_UP)); // player hit 
      dealer->receive_card(card_deck->deal_card(Card::DEALER_FACE_UP)); // dealer hit 

      assert(player->get_num_cards() == 3);
      assert(dealer->get_num_cards() == 3);

      // ignore result and discard
      discard_cards();
    }

    return 0;
  }
  
};

int main() {

  GameTester *gt;
  int num_decks = 3;
  int shoe = 20;

  gt = new GameTester(num_decks, shoe);
  if (gt->test_hand_result() == 0)
    cout << "*** Passed hand result test ***" << endl;
  else
    cout << "*** Failed hand result test ***" << endl;
  delete gt;


  gt = new GameTester(num_decks, shoe);
  // could send problems to stderr and monitor that, this test causes a 
  // lot of printouts on purpose
  if (gt->test_dealer_logic() == 0)
    cout << "*** Passed dealer logic test ***" << endl;
  else
    cout << "*** Failed dealer logic test ***" << endl;
  delete gt;


  gt = new GameTester(num_decks, shoe);
  if (gt->test_deal_discard_shuffle() == 0)
    cout << "*** Passed deal_discard_shuffle test ***" << endl;
  else
    cout << "*** Failed deal_discard_shuffle test ***" << endl;
  delete gt;


  return 0;
}
