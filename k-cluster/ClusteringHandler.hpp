#include <vector>
#include "Point.hpp"
#include <random>

using namespace std;
#ifndef _CLUSTERINGHANDLER_HPP
#define _CLUSTERINGHANDLER_HPP
class ClusteringHandler{

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
   *
   */
  int rng_seed;

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
  vector<double> mean_square_error;

  /**
   * number of iterations completed (1 after first iter completed)
   */
  int num_iter;

  /**
   * small change in cost required to continue iterations
   */
  double round_delta;

  /**
   * Random generator
   */
  std::default_random_engine gen;

  /**
   * Uniform double random 
   */
  std::uniform_real_distribution<double> unif_dist;
  std::uniform_real_distribution<double> unit_unif_dist;

  /**
   * Normal distribution
   */
  std::normal_distribution<> norm_dist; 

  /**
   * bimodal distance between modes in same dimension
   */
  double bimodal_dist;

public:

  ClusteringHandler(int k_a, double round_delta_a, int rng_seed_a) {
    k = k_a;
    round_delta = round_delta_a;
    num_iter = 0;
    rng_seed = rng_seed_a;   
    gen = std::default_random_engine(rng_seed);
    norm_dist = std::normal_distribution<>(0,1); // 0: mean, 1 var
    unif_dist = std::uniform_real_distribution<double>(-1*max_pt_val, max_pt_val);
    unit_unif_dist = std::uniform_real_distribution<double>(0,1);
    bimodal_dist = 3;
  
  }

  ClusteringHandler(int k_a) {
    k = k_a;
    num_iter = 0;
    norm_dist = std::normal_distribution<>(0,1); // 0: mean, 1 var
    unif_dist = std::uniform_real_distribution<double>(-1*max_pt_val, max_pt_val);
    unit_unif_dist = std::uniform_real_distribution<double>(0,1);
    bimodal_dist = 3;
  }

  ClusteringHandler() {
    num_iter = 0;
    k = 2;
    norm_dist = std::normal_distribution<>(0,1); // 0: mean, 1 var
    unif_dist = std::uniform_real_distribution<double>(-1*max_pt_val, max_pt_val);
    unit_unif_dist = std::uniform_real_distribution<double>(0,1);
    bimodal_dist = 3;
  }

  ~ClusteringHandler() {}

  double get_mean_square_error() {

    return mean_square_error[mean_square_error.size()-1];
  }
  
  int get_num_rounds() {

    return mean_square_error.size()-1;
  }

  void print_round() {
    assert(mean_square_error.size() == num_iter);

    if (num_iter == 0) {
      cout << "No rounds yet" << endl << endl;
      return;
    }

    cout << "---- Round " << num_iter << " ---- " << endl;
    cout << "centroids are at: " << endl;
   
    // c++-0x, OS X does not have this compiler :(
    for (auto &cent_i : centroids) {
      cent_i.print_point();
    }

    cout << "MSE is: " << mean_square_error[num_iter-1] << endl;
    cout << endl;

  }

  void run() {

    perform_initial_association();
    int prev_cost = mean_square_error[mean_square_error.size()-1];
    perform_round();
    int curr_cost = mean_square_error[mean_square_error.size()-1];
    while (curr_cost < prev_cost - round_delta) {
      prev_cost = curr_cost;
      perform_round();
      curr_cost = mean_square_error[mean_square_error.size()-1];
    }
  }

  void perform_initial_association() {

    init_centroids();
    associate_pts(); 
    calc_mse_last_round();
    num_iter++;
  }

  void perform_round() {

    recompute_centroids();
    associate_pts();
    calc_mse_last_round();
    num_iter++;
  }

  void calc_mse_last_round() {

    assert(mean_square_error.size() == num_iter);

    double mse = 0;
    for (int pt_i = 0; pt_i < pts.size(); pt_i++) {
      
      int cent_idx = assoc[pt_i];
      double dist = compute_distance(centroids[cent_idx], pts[pt_i]);
      mse += (dist)*(dist) / num_pts;
    }
    mean_square_error.push_back(mse);
  }



  void gen_unif_rnd_indep_dim_pts(int num_pts_a, int num_dim_a) {

    num_pts = num_pts_a;
    num_dim = num_dim_a;

    for (int i = 0; i < num_pts; i++) {
      pts.push_back(gen_unif_rnd_indep_dim_pt());
    }
    assoc.resize(num_pts);
  }

  void gen_bimodal_gauss_rnd_indep_dim_pts(int num_pts_a, int num_dim_a) {

    num_pts = num_pts_a;
    num_dim = num_dim_a;

    for (int i = 0; i < num_pts; i++) {
      pts.push_back(gen_bimodal_gauss_rnd_indep_dim_pt());
    }
    assoc.resize(num_pts);
  }

  void gen_gauss_rnd_indep_dim_pts(int num_pts_a, int num_dim_a) {

    num_pts = num_pts_a;
    num_dim = num_dim_a;

    for (int i = 0; i < num_pts; i++) {
      pts.push_back(gen_gauss_rnd_indep_dim_pt());
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

    for (int pt_i = 0; pt_i < pts.size(); pt_i++) {
      int cent_idx = get_nearest_centroid_idx(pts[pt_i]);
      assoc[pt_i] = cent_idx;
    }

  }

  /**
   * takes current associations and recomputes centroids based on
   * their center
   * uses centroids, pts, assoc, and k to rec
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
   
    // c++ 0x
    for (auto &cent_i : centroids){
      auto idx = &cent_i - &centroids[0];
      double dist = compute_distance(pt, cent_i);
      if (dist < min_dist) {
    	cent_idx = idx;
    	min_dist = dist;
      }
    }
    
    return cent_idx;
  }

  Point gen_bimodal_gauss_rnd_indep_dim_pt() {
    vector<double> vals;
    for (int i = 0; i < num_dim; i++) {
      if (unit_unif_dist(gen) > 0.5)
	vals.push_back(norm_dist(gen) + bimodal_dist/2);
      else
	vals.push_back(norm_dist(gen) - bimodal_dist/2);
    }
    
    Point pt(vals);

    return pt;
  }  

  Point gen_unif_rnd_indep_dim_pt() {
    vector<double> vals; 

    for (int i = 0; i < num_dim; i++) {
      vals.push_back(unif_dist(gen));
    }

    Point pt(vals);

    return pt;
  }

  Point gen_gauss_rnd_indep_dim_pt() {
    vector<double> vals;

    for (int i = 0; i < num_dim; i++) {
      vals.push_back(norm_dist(gen));
      cout << vals[i] << endl;
    }
    
    Point pt(vals);

    return pt;
  }


};
#endif
