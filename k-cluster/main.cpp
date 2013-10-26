#include <iostream>
#include <vector>
#include <stdlib.h>
#include <assert.h> 

#define MAX_VAL 200

using namespace std;

template <class T>
class Point {
private:
  int num_dim;
  vector<T> vals;

public:
  Point(vector<T> vals_a){
    num_dim = vals_a.size();
    vals = vals_a;
  }

  int get_num_dim() {
    return num_dim;
  }
  T get_value(int idx) {
    assert(idx < num_dim && idx >= 0);
    return vals[idx];
  }

};

template <class T>
class ClusteringDriver{

private:
  /**
   * k centroids
   */
  int k; 

  /**
   * number of points
   */
  int num_pts;

  /**
   * vector of points
   */
  vector<Point<T> > pts;

public:

  ClusteringDriver(int k_a, vector<Point<T> > pts_a) {
    k = k_a;
    num_pts = pts_a.size();

    pts = pts_a;
  }
  
  
};

Point<int> gen_rand_2d_int_pt(int num_dim) {

  vector<int> vals;
  for (int i = 0; i < num_dim; i++) {
    vals.push_back(rand() % MAX_VAL);
  }

  Point<int> pt(vals);

  return pt;
}

vector<Point<int> > gen_rand_2d_int_pts(int num_dim, int num_pts) {

  vector<Point<int> > pts;
  for (int i = 0; i < num_pts; i++) {
    pts.push_back(gen_rand_2d_int_pt(num_dim));
  }

  return pts;
}

int main() {

  ClusteringDriver<int> *cd;
  
  int k_centroids = 4;
  int num_dim = 2;
  int num_pts = 100;

  vector<Point<int> > pts;
  pts = gen_rand_2d_int_pts(num_dim, num_pts);

  cd = new ClusteringDriver<int>(k_centroids, pts);

  return 0;
}
