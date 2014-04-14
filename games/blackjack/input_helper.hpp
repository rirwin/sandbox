#ifndef __INPUT_HELPER__
#define __INPUT_HELPER__

#include <iostream>
#include <string>

using namespace std;

int get_int_from_user(string prompt, int lower_bound, int upper_bound) {
  
  string raw_input;
  int formatted_input = 0;

  // used for checking if valid input provided
  char* p_end = NULL;

  do {
    cout << prompt << endl;
    getline(cin, raw_input);
    formatted_input = strtol(raw_input.c_str(), &p_end, 10);
    if (*p_end != '\0'){
      cout << "Malformed input, try again" << endl;
    }
    else if (formatted_input < lower_bound || formatted_input > upper_bound) {
      cout << "Please enter an integer between " << lower_bound << " and " << upper_bound << endl;
    }
  }  while(formatted_input < lower_bound || formatted_input > upper_bound || *p_end != '\0');

  return formatted_input;
}

char get_y_or_n_from_user(string prompt) {
  
  string raw_input;

  // ensure enter loop and allocated non-garbage value
  char formatted_input = ' ';

  while(formatted_input != 'y' && formatted_input != 'n') {

    cout << prompt << endl;
    getline(cin, raw_input);

    if (raw_input.length() > 1)
      cout << "Malformed input, try again" << endl;
    else {
      formatted_input = raw_input[0];
      if (formatted_input != 'y' && formatted_input != 'n') 
	cout << "Please enter y or n" << endl;
    }
  }

  return formatted_input;
}

#endif
