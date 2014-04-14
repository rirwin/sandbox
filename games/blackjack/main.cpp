#include <iostream>
#include <vector>

#include "card.hpp"
#include "game.hpp"
#include "input_helper.hpp"

using namespace std;

bool check_play_again();

int main() {

  int num_decks = 1;
  int shuffle_shoe = 15;
  Game game(num_decks, shuffle_shoe);

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

  string prompt = "Would you like to keep playing? (y/n) ";
  char play_again = get_y_or_n_from_user(prompt);
  if (play_again == 'y')
    return true;
  else
    return false;
}
