#include <iostream>
#include <assert.h>
#include <vector>

#include "../card.hpp"
#include "../card_deck.hpp"
#include "../player.hpp"

using namespace std;


// class used for testing of state of protected variables of
// inherited class Player (no need for friend class)
class PlayerTester : public Player {

public:
  PlayerTester(player_type_enum player_type) : Player(player_type){
  }

  int test_has_blackjack() {
    
    for (int level_i = 1; level_i < 14; level_i++) {
      for (int level_j = 1; level_j < 14; level_j++) {
	
	// ensures no cards in hand
	cards.clear(); 

	Card *c1 = new Card('S',level_i);
	Card *c2 = new Card('H',level_j);

	receive_card(c1);
	receive_card(c2);

	if (c1->get_value() + c2->get_value() == 21) 
	  assert(has_blackjack() == true);
	else
	  assert(has_blackjack() == false);

	delete c1;
	delete c2;
      }
    }

    return 0;
  }

  // Try this first then generalize to n card hand
  int test_two_card_hand_value() {

    for (int level_i = 1; level_i < 14; level_i++) {
      for (int level_j = 1; level_j < 14; level_j++) {
	
	// ensures no cards in hand
	cards.clear(); 
	
	Card *c1 = new Card('S',level_i);
	Card *c2 = new Card('H',level_j);
	
	receive_card(c1);
	receive_card(c2);
	int sum_vals = c1->get_value() + c2->get_value();
	if (sum_vals <= 21) {
	  assert(get_hand_value() == sum_vals);
	}
	else { 
	  int num_aces = get_num_aces_in_hand();
	  while (num_aces > 0 && sum_vals > 21) {
	    sum_vals-=10;
	    num_aces--;
	  }
	  assert(get_hand_value() == sum_vals);
	}
	delete c1; delete c2; 
      }
    }
	
    
    return 0;
  }  

  // Try this first then generalize to n card hand
  int test_three_card_hand_value() {

    for (int level_i = 1; level_i < 14; level_i++) {
      for (int level_j = 1; level_j < 14; level_j++) {
	for (int level_k = 1; level_j < 14; level_j++) {
	
	  // ensures no cards in hand
	  cards.clear(); 
	  
	  Card *c1 = new Card('S',level_i);
	  Card *c2 = new Card('H',level_j);
	  Card *c3 = new Card('D',level_k);
	  
	  receive_card(c1);
	  receive_card(c2);
	  receive_card(c3);
	  
	  int sum_vals = c1->get_value() + c2->get_value() + c3->get_value();
	  if (sum_vals <= 21) {
	    assert(get_hand_value() == sum_vals);
	  }
	  else { 
	    int num_aces = get_num_aces_in_hand();
	    while (num_aces > 0 && sum_vals > 21) {
	      sum_vals-=10;
	      num_aces--;
	    }
	    assert(get_hand_value() == sum_vals);
	  }
	  delete c1; delete c2; delete c3;
	}
      }
    }
	
    return 0;
  }

  // Try this first then generalize to n card hand
  int test_four_card_hand_value() {

    for (int level_i = 1; level_i < 14; level_i++) {
      for (int level_j = 1; level_j < 14; level_j++) {
	for (int level_k = 1; level_k < 14; level_k++) {
	  for (int level_l = 1; level_l < 14; level_l++) {
	
	    // ensures no cards in hand
	    cards.clear(); 
	    
	    Card *c1 = new Card('S',level_i);
	    Card *c2 = new Card('H',level_j);
	    Card *c3 = new Card('D',level_k);
	    Card *c4 = new Card('D',level_l);
	    
	    receive_card(c1);
	    receive_card(c2);
	    receive_card(c3);
	    receive_card(c4);
	    
	    int sum_vals = c1->get_value() + c2->get_value() + c3->get_value() + c4->get_value();
	    if (sum_vals <= 21) {
	      assert(get_hand_value() == sum_vals);
	    }
	    else { 
	      int num_aces = get_num_aces_in_hand();
	      while (num_aces > 0 && sum_vals > 21) {
		sum_vals-=10;
		num_aces--;
	      }
	      assert(get_hand_value() == sum_vals);
	    }
	    delete c1; delete c2; delete c3; delete c4;
	  }
	}
      }
    }
    return 0;
  }

  // Try this first then generalize to n card hand
  int test_five_card_hand_value() {

    for (int level_i = 1; level_i < 14; level_i++) {
      for (int level_j = 1; level_j < 14; level_j++) {
	for (int level_k = 1; level_k < 14; level_k++) {
	  for (int level_l = 1; level_l < 14; level_l++) {
	    for (int level_n = 1; level_n < 14; level_n++) {
	
	      // ensures no cards in hand
	      cards.clear(); 
	      
	      Card *c1 = new Card('S',level_i);
	      Card *c2 = new Card('H',level_j);
	      Card *c3 = new Card('D',level_k);
	      Card *c4 = new Card('D',level_l);
	      Card *c5 = new Card('D',level_n);
	      
	      receive_card(c1);
	      receive_card(c2);
	      receive_card(c3);
	      receive_card(c4);
	      receive_card(c5);
	      
	      int sum_vals = c1->get_value() + c2->get_value() + c3->get_value() + c4->get_value() + c5->get_value();
	      if (sum_vals <= 21) {
		assert(get_hand_value() == sum_vals);
	      }
	      else { 
		int num_aces = get_num_aces_in_hand();
		while (num_aces > 0 && sum_vals > 21) {
		  sum_vals-=10;
		  num_aces--;
		}
		assert(get_hand_value() == sum_vals);
	      }
	      delete c1; delete c2; delete c3; delete c4; delete c5;
	    }
	  }
	}
      }
    }
    return 0;
  }

  int get_num_aces_in_hand() {
    int cnt = 0;
    for (int i = 0; i < cards.size(); i++)
      if (cards[i]->get_level() == 1)
	cnt++;
    return cnt;
  }
  
};
  
int main() {

  cout <<"STARTING PLAYER TESTS ----------------" << endl;

  PlayerTester *pt;

  // Test initial chip count ------
  pt = new PlayerTester(Player::PLAYER);
  assert(pt->get_chip_count() == 100);
  delete pt;

  pt = new PlayerTester(Player::DEALER);
  assert(pt->get_chip_count() == 0);
  delete pt;

  // Test blackjack -----
  pt = new PlayerTester(Player::PLAYER);
  if (pt->test_has_blackjack() == 0)
    cout << "Passed blackjack test" << endl;
  else
    cout << "Failed blackjack test" << endl;
  delete pt;

  // Test hand values -----
  pt = new PlayerTester(Player::PLAYER);
  if (pt->test_two_card_hand_value() == 0)
    cout << "Passed two card hand value test" << endl;
  else
    cout << "Failed two card hand value test" << endl;
  delete pt;
  
  pt = new PlayerTester(Player::PLAYER);
  if (pt->test_three_card_hand_value() == 0)
    cout << "Passed three card hand value test" << endl;
  else
    cout << "Failed three card hand value test" << endl;
  delete pt;

  pt = new PlayerTester(Player::PLAYER);
  if (pt->test_four_card_hand_value() == 0)
    cout << "Passed four card hand value test" << endl;
  else
    cout << "Failed four card hand value test" << endl;
  delete pt;
  
  pt = new PlayerTester(Player::PLAYER);
  if (pt->test_five_card_hand_value() == 0)
    cout << "Passed five card hand value test" << endl;
  else
    cout << "Failed five card hand value test" << endl;
  delete pt;
  

  return 0;
}
