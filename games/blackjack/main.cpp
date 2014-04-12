
#include <iostream>
#include "card.hpp"
#include "game.hpp"
#include <vector>
using namespace std;

bool check_play_again();

int main() {

  bool play_again = true;
  int chips = 100;

  Game game(3);
  game.play_new_hand();
  //Card c = game.deal_card();
  //c.print();

  while(play_again) {
    
    // Get input from the player

    // update the world

    // draw graphics

    // repeat until quite
    play_again = check_play_again();
  }

  cout << "Thank you for playing" << endl;

  return 0;
}



bool check_play_again() {

  Card c('D',9);
  //cout << c.get_card_state()<< endl;
  //cout << Card::DISCARD << endl;
  char input = ' ';

  while (input != 'y' && input != 'n') {
    cout << "Would you like to keep playing? (y/n) " << endl;
    cin >> input;
  }
  if (input == 'y') {
    return true;
  }
  else if (input == 'n') {
    return false;
  }
  else {
    cout << "Please enter y or n" << endl;
  }
}
