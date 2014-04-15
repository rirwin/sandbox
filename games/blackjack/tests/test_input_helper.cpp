#include <iostream>

#include "../input_helper.hpp"

using namespace std;

// from input_helper.hpp, this program tests
// tests get_int_from_user() and 
// tests get_y_or_n_from_user()

int main() {

  bool continue_test = true;
  while(continue_test) {
    int input = get_int_from_user("give me an int between -10 and 10", -10, 10);
    cout << "You gave " << input << endl;
    char y_or_n = get_y_or_n_from_user("Continue Testing integer input (y/n)?");
    if (y_or_n == 'n')
      continue_test = false;
  }

  continue_test = true;
  while(continue_test) {
    char y_or_n = get_y_or_n_from_user("Give me a y or n");
    cout << "You gave " << y_or_n << endl;
    y_or_n = get_y_or_n_from_user("Continue Testing y or n input (y/n)");
    if (y_or_n == 'n')
      continue_test = false;
  }
  
  return 0;
}
