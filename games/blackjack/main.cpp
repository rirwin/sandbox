
#include <iostream>
#include "card.hpp"
#include "game.hpp"
#include <vector>
using namespace std;

bool check_play_again();

int main() {

  int num_decks = 3;
  Game game(num_decks);

  cout << "Welcome to the game of blackjack!" << endl;

  bool play_again = true;
  while(play_again) {

    game.play_new_hand();

    int chips = game.get_player_chip_count();
    cout << "You have " << chips << " chips" << endl;

    if (chips > 0)
      play_again = check_play_again();
    else {
      cout << "You are out of chips." << endl;
      play_again = false;
    }
  }

  cout << "Thank you for playing!" << endl;

  return 0;
}


bool check_play_again() {

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

  return false;
}
