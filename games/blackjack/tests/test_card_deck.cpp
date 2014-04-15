#include <iostream>
#include <assert.h>
#include "../card_deck.hpp"

using namespace std;

// class used for testing of state of protected variables of
// inherited class CardDeck (no need for friend class)
class CardDeckTester : public CardDeck {
public:
  CardDeckTester(int num_decks) : CardDeck(num_decks){
  }
  
  int deck_init_tester() {
    assert(this->get_num_cards_left() == num_decks*52);
    shuffle();
    for (int card_idx = 0; card_idx < 52*num_decks; card_idx++)
      assert(cards[card_idx].get_state() == Card::DECK);
    return 0;
  }

  int deal_tester() {

    Card *c1, *c2, *c3, *c4;

    // Deal exactly all the cards
    for (int card_grp_idx = 0; card_grp_idx < 52*num_decks/4; card_grp_idx++) {
      assert(get_num_cards_left() == 52*num_decks - card_grp_idx*4);
      assert(next_card_idx == card_grp_idx*4);
      c1 = deal_card(Card::PLAYER_FACE_UP);
      c2 = deal_card(Card::PLAYER_FACE_UP);
      c3 = deal_card(Card::DEALER_FACE_DOWN);
      c4 = deal_card(Card::DEALER_FACE_UP);

      assert(c1->get_state() == Card::PLAYER_FACE_UP);
      assert(c2->get_state() == Card::PLAYER_FACE_UP);
      assert(c3->get_state() == Card::DEALER_FACE_DOWN);
      assert(c4->get_state() == Card::DEALER_FACE_UP);

      c1->set_state(Card::DISCARD);
      c2->set_state(Card::DISCARD);
      c3->set_state(Card::DISCARD);
      c4->set_state(Card::DISCARD);

      assert(c1->get_state() == Card::DISCARD);
      assert(c2->get_state() == Card::DISCARD);
      assert(c3->get_state() == Card::DISCARD);
      assert(c4->get_state() == Card::DISCARD);

    }
    return 0;
  }

};
  
int main() {

  for (int num_decks_i = 1; num_decks_i < 10; num_decks_i++) {
    CardDeckTester *d = new CardDeckTester(num_decks_i);
    if (d->deck_init_tester() == 0)
      cout << "Passed deck init tester" << endl;
    else
      cout << "Failed deck init tester" << endl;

    if (d->deal_tester() == 0)
      cout << "Passed deal tester" << endl;
    else
      cout << "Failed deal tester" << endl;


    delete d;
  }

    
}
