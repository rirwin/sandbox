#include "../card.hpp"
#include <iostream>
#include <assert.h>

using namespace std;

// class used for testing of state of protected variables of
// inherited class Card (no need for friend class)
class CardTester : public Card {
public:
  CardTester(char suit, int level): Card(suit, level) {
  }

  int test_card_state_changes() {

    assert(get_state() == Card::DECK);
    
    
    set_state(Card::PLAYER_FACE_UP);
    assert(get_state() == Card::PLAYER_FACE_UP);
    
    
    set_state(Card::DEALER_FACE_DOWN);
    assert(get_state() == Card::DEALER_FACE_DOWN);
    
    
    set_state(Card::DEALER_FACE_UP);
    assert(get_state() == Card::DEALER_FACE_UP);
    
    
    set_state(Card::DISCARD);
    assert(get_state() == Card::DISCARD);
    
    return 0;
  }

};

// Doesn't need to be inside a CardTest class
int test_card_init1() {

  Card *c = NULL;
  try {
    c = new Card('S',10);
  }
  catch (char const *err_str) {
    
  }
  
  if (c != NULL)
    delete c;
  c = NULL;
  
  try {
    c = new Card('?',10);
  }
  catch (char const *err_str) {
    
  }
  
  if (c != NULL)
    delete c;
  c = NULL;
    
  try {
    c = new Card('S',100);
  }
  catch (char const *err_str) {
    
  }
  
  if (c != NULL)
    delete c;
  c = NULL;
  
  cout << "All exceptions handled properly" << endl;

  return 0;
}

// Doesn't need to be inside a CardTest class
int test_card_level_value_translation() {
  Card *c = NULL;
  for (int i = 1; i < 14; i++) {
    c = new Card('H',i);
    if (c->get_level() >= 2 && c->get_level() <= 10)
      assert(c->get_value() == c->get_level());
    else if (c->get_level() == 1) 
      assert(c->get_value() == 11);
    else if (c->get_level() >= 11 || c->get_level() <= 13)
      assert(c->get_value() == 10);
    else
      assert(false); // should never reach here
  }
  return 0;
}

int main() {

  if (test_card_init1() == 0)
    cout << "Passed card_init1 test" << endl;
  else
    cout << "Failed card_init1 test" << endl;
  
  if (test_card_level_value_translation() == 0)
    cout << "Passed card_level_value_translation test" << endl;
  else
    cout << "Failed card_level_value_translation" << endl;


  CardTester *ct = new CardTester('S',1);
  if (ct->test_card_state_changes() == 0)
    cout << "Passed card_state_changes test" << endl;
  else
    cout << "Failed card_state_changes test" << endl;
  delete ct;


  return 0;
}

