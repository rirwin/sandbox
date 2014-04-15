#include "../card.hpp"
#include <iostream>
#include <assert.h>

using namespace std;

int main() {


  // Testing card initialization ------------------
  Card *c = NULL;
  try {
    c = new Card('S',10);
    c->print();
    cout << endl;
  }
  catch (char const *err_str) {
    cout << *err_str << endl;
  }

  if (c != NULL)
    delete c;
  c = NULL;

  try {
  c = new Card('?',10);
  c->print();
  cout << endl;
  }
  catch (char const *err_str) {
    cout << *err_str << endl;
  }

  if (c != NULL)
    delete c;
  c = NULL;

  try {
    c = new Card('S',100);
    c->print();
    cout << endl;
  }
  catch (char const *err_str) {
    cout << *err_str << endl;
  }

  if (c != NULL)
    delete c;
  c = NULL;

  // Testing card state --------------------
  try {
    c = new Card('S',1);
  }  
  catch (char const *err_str) {
    cout << *err_str << endl;
  }

  assert(c->get_state() == Card::DECK);
  c->print_state();

  c->set_state(Card::PLAYER_FACE_UP);
  assert(c->get_state() == Card::PLAYER_FACE_UP);
  c->print_state();

  c->set_state(Card::DEALER_FACE_DOWN);
  assert(c->get_state() == Card::DEALER_FACE_DOWN);
  c->print_state();

  c->set_state(Card::DEALER_FACE_UP);
  assert(c->get_state() == Card::DEALER_FACE_UP);
  c->print_state();

  c->set_state(Card::DISCARD);
  assert(c->get_state() == Card::DISCARD);
  c->print_state();

  return 0;
}
