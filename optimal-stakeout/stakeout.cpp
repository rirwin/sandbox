/**
 * Stakeout problem
 *
 * Congratulations!  You are the new elite hacker in a group of
 * villainous ne'er-do-wells.
 *
 * Luckily this group is more saavy than your last band of ruffians, and
 * they are looking to software (and you) to improve their take. The con
 * man for the team, has gone door-to-door down each street posing as a
 * termite inspector so he could covertly total the valuable goods in
 * each house. Normally the gang would just rob all the valuable homes,
 * but there's a catch! Whenever a house is robbed in this wealthy
 * neighborhood, the police watch it and the neighboring houses for
 * months.
 *
 * So the gang can't simply rob all the homes, and if they choose to rob
 * one, they can no longer rob the house on either side of it.
 *
 * The ringleader wants to know what houses he should rob to maximize the
 * team's profit, and he wants to know now. Write a function that takes
 * in an array of positive integers (home values) and returns the maximum
 * expected value of robbing that street.
 *
 * For example:
 *
 * [ 20, 10, 50, 5, 1 ] should return $71, as robbing the first,
 * third, and fifth houses is optimal [ 20, x, 50, x, 1 ]
 *
 * [ 20, 50, 10, 1, 5 ] should return $55, as robbing the second and
 * fifth houses is optimal [ x, 50, x, x, 5 ]
 *
 * To solve this problem, we use recursion.  Recursively solve for the max of including the i^th house or the (i+1)^th house.  While solving for the i^th or (i+1)^th house, the array that is solved gets smaller and smaller with each recursive call down to the base cases.  
 *
 * The problem with the above method is that there may be duplicate calls to the stakeout solver function (i.e., calling the function with the same value for idx or i^th house).  These duplicate calls are unavoidable but we can cache the results from the first call with a particular idx, which prevents recursing down a path that has already been explored.
 */

#include <iostream>
#include <stdlib.h>

using namespace std;

/**
 * a is array of house values
 * n is the length of array
 * idx is the index to look at the array a[idx : end]
 * c is the cache of previously used results
 */
int stakeout_solver_value_dp(int *a, int n, int idx, int *c){

  // base case: out of bounds
  //
  if (idx >= n)
    return 0;
  
  // base case: end of the array
  //
  if (idx == n - 1)
    return a[idx];
  
  // base case: last two elements of array, return max of the two
  //
  if (idx == n - 2)
    return max(a[idx], a[idx+1]);

  // Look in cache c for previous result (-1 indicates not in cache).
  // If not in cache, make recursive calls and take the max of the result.
  // The *first* argument to the max function is to include house idx and 
  // recurse on the rest of the array skipping the next house (idx + 2).
  // The *second* argument of the max function is to include house idx + 1 and
  // recurse on the rest of the array skipping the next two houses (idx + 3).
  //
  if (c[idx] == -1)   
    c[idx] = max(a[idx] + stakeout_solver_value_dp(a, n, idx + 2, c),
		 a[idx + 1] + stakeout_solver_value_dp(a, n, idx + 3, c));
  
  // return the maximum value that was cached or calculated in the max() funcition.
  //
  return c[idx];
}


int main(int argv, char *argc[]) { 

  int *house_values;
  int *cache;
  int num_house_values = 0;

  // ----- Argument checking -----
  //
  if (argv < 2) {
    cout << "Please provide arguments of house values in a list." << endl;
    cout << "For example ./stakeout 20 10 50 5 1" << endl;
    cout << endl;

    // I decided to give default values if no values were provided
    //
    cout << "The following default values will be used instead: "<< endl;
    cout << endl;
    cout << "[20 10 50 5 1]" << endl; 
    num_house_values = 5;
    house_values = (int*) malloc(num_house_values*sizeof(int));
    cache = (int*) malloc(num_house_values*sizeof(int));
    house_values[0] = 20;
    house_values[1] = 10;
    house_values[2] = 50;
    house_values[3] = 5;
    house_values[4] = 1;
  }
  else {
    num_house_values = argv - 1;
    house_values = (int*) malloc(num_house_values*sizeof(int));
    cache = (int*) malloc(num_house_values*sizeof(int));
    cout << "Num values provided " << num_house_values << endl;
    char *endptr = NULL;  

    for (int arg_i = 0; arg_i < num_house_values; arg_i++) {
    
      // parse house value for argc the safe way
      // arg_i + 1 accounts for c-style command line arguments
      //
      int house_value_i = strtol(argc[arg_i + 1], &endptr, 10);
      if (*endptr != 0){
	cout << "Non-integer argument, exiting" << endl;
	return -1;
      }

      // check if non-positive
      if (house_value_i <= 0) {
	cout << "Positive integer required" << endl;
	return -1;
      }

      // input is okay, assign it to array
      //
      house_values[arg_i] = house_value_i;
    }
  }

  // ----- End argument checking -----

  // initialize cache, -1 indicates nothing cached of cache[idx]
  //
  for (int idx = 0; idx < num_house_values; idx++)
    cache[idx] = -1;

  cout << endl;
  cout << "House values from reconaissance: ";
  for (int idx = 0; idx < num_house_values; idx++)
    cout << house_values[idx] << " ";
  cout << endl;
  cout << "Expected Value:" << endl <<stakeout_solver_value_dp(house_values, num_house_values, 0, cache) << endl;
  
  return 0;
}
