#include <vector>

using namespace std;

#ifndef _POINT_HPP
#define _POINT_HPP

const int max_pt_val = 200;


class Point{
private:
  int num_dim;
  vector<double> vals;

public:
  Point(vector<double> vals_a){
    num_dim = vals_a.size();
    vals = vals_a;
  }

  int get_num_dim() {
    return num_dim;
  }
  double get_value(int idx) {
    assert(idx < num_dim && idx >= 0);
    return vals[idx];
  }
  void update_point(vector<double> new_vals) {
    assert(new_vals.size() == vals.size());
    for (int i = 0; i < vals.size(); i++) {
      vals[i] = new_vals[i];
    }
  }
  void print_point() {
    cout << "(";
    for (int i = 0; i < vals.size()-1; i++){
      cout << " " << vals[i] << ",";
    }
    cout << " " << vals[vals.size()-1] << " )"<<endl;
  }

};
#endif
