#include <iostream>
#include <vector>
#include <stdlib.h>
#include <assert.h> 
#include <math.h>
#include <cfloat>
#define MAX_VAL 200

using namespace std;


class Point {
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

class ClusteringDriver{

private:
  /**
   * k centroids
   */
  int k; 

  /**
   *
   */
  int num_dim;

  /**
   * number of points
   */
  int num_pts;

  /**
   * vector of points
   */
  vector<Point> pts;

  /** 
   * current centroids
   */
  vector<Point> centroids;
  
  /**
   * current association of points to centroids
   */
  vector<int> assoc;

  /**
   * cost of clustering per run used to check if converged and useful
   * for see if clustering with k centroids was a good idea
   */
  vector<double> clust_costs;

  /**
   * number of iterations completed (1 after first iter completed)
   */
  int num_iter;

public:

  ClusteringDriver(int k_a) {
    k = k_a;
    num_iter = 0;
  }

  ClusteringDriver() {
    num_iter = 0;
  }

  void print_round() {
    assert(clust_costs.size() == num_iter);

    if (num_iter == 0) {
      cout << "No rounds yet" << endl << endl;
      return;
    }

    cout << "---- Round " << num_iter << " ---- " << endl;
    cout << "centroids are at: " << endl;
    for (auto &cent_i : centroids) {
      cent_i.print_point();
    }

    cout << "cost is: " << clust_costs[num_iter-1] << endl;
    cout << endl;

  }

  void gen_rnd_pts(int num_pts_a, int num_dim_a) {

    num_pts = num_pts_a;
    num_dim = num_dim_a;
    
    for (int i = 0; i < num_pts; i++) {
      pts.push_back(gen_rand_pt());
    }
    assoc.resize(num_pts);
  }

  void init_centroids() {
    
    // Initial k centroids are first k pts to start
    assert(num_pts > k);
    for (int i = 0; i < k; i++) {
      centroids.push_back(pts[i]);
    }
  }

  /**
   * Associates points and provides cost as summed distance
   */
  void associate_pts() {

    assert(clust_costs.size() == num_iter);
    clust_costs.push_back(0);

    for (auto &pt_i : pts) {
      int pt_idx = &pt_i - &pts[0];
      int cent_idx = get_nearest_centroid_idx(pt_i);
      assoc[pt_idx] = cent_idx;
    }

    num_iter++;
  }


  /**
   * takes current associations and recomputes centroids based on
   * their center
 // uses centroids, pts, assoc, and k to rec
   */
  void recompute_centroids() {
    for (int i = 0; i < k; i++)
      recompute_centroid_idx(i);
  }

  /**
   * recomputes ith centroid
   */
  void recompute_centroid_idx(int cent_idx) {
    vector<double> sums;
    int ctr = 0;
    sums.resize(num_dim);
    for (auto &i : sums){i = 0;}

    for (int pt_i = 0; pt_i < num_pts; pt_i++) {
      if (assoc[pt_i] == cent_idx) {
	ctr++;
	for (int dim_i = 0; dim_i < num_dim; dim_i++) {
	  sums[dim_i] += pts[pt_i].get_value(dim_i);
	}
      }
    }
    for (int dim_i = 0; dim_i < num_dim; dim_i++) 
      sums[dim_i] /= ctr;

    centroids[cent_idx].update_point(sums);
  }
  
  double compute_distance(Point pt1, Point pt2){

    assert(num_dim == pt1.get_num_dim() && num_dim == pt2.get_num_dim());
    double dist_sq = 0;
    for (int dim_i = 0; dim_i < num_dim; dim_i++) {
      dist_sq += pow(pt1.get_value(dim_i) - pt2.get_value(dim_i),2);
    }
    
    return sqrt(dist_sq);
  }
  
  int get_nearest_centroid_idx(Point pt) {

    double min_dist = DBL_MAX;
    int cent_idx = -1;

    for (auto &cent_i : centroids){
      auto idx = &cent_i - &centroids[0];
      double dist = compute_distance(pt, cent_i);
      if (dist < min_dist) {
	cent_idx = idx;
	min_dist = dist;
      }
    }
    clust_costs[num_iter] += min_dist;
    
    return cent_idx;
  }

  Point gen_rand_pt() {
    vector<double> vals;
    for (int i = 0; i < num_dim; i++) {
      vals.push_back(rand() % MAX_VAL);
    }
    
    Point pt(vals);

    return pt;
  }

};





int main() {

  ClusteringDriver *cd;
  
  int k_centroids = 4;
  int num_dim = 2;
  int num_pts = 100;
  int num_rounds = 10;

  cd = new ClusteringDriver(k_centroids);
  cd->gen_rnd_pts(num_pts, num_dim);
  cd->init_centroids();
  cd->associate_pts();
  cd->print_round();


  for (int i = 0; i < num_rounds; i++) {
    cd->recompute_centroids();
    cd->associate_pts();
    cd->print_round();
  }

  return 0;
}
