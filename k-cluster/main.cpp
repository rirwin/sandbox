#include <iostream>
#include <vector>
#include <stdlib.h>
#include <assert.h> 
#include <math.h>
#include <cfloat>
#include <getopt.h>

const int max_pt_val = 200;

using namespace std;

/**
 * Parsing/usage functions
 */
char *progname;
static void usage();
static void parse_args (int argc, char *argv[], int *num_points, int *num_dimensions, double *round_delta, int *num_clusters, int *rng_seed);


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
  vector<double> clust_costs;

  /**
   * number of iterations completed (1 after first iter completed)
   */
  int num_iter;

  /**
   * small change in cost required to continue iterations
   */
  double round_delta;

public:

  ClusteringDriver(int k_a, double round_delta_a) {
    k = k_a;
    round_delta = round_delta_a;
    num_iter = 0;
  }

  ClusteringDriver(int k_a) {
    k = k_a;
    num_iter = 0;
  }

  ClusteringDriver() {
    num_iter = 0;
    k = 2;
  }

  double get_final_cost() {

    return clust_costs[clust_costs.size()-1];
  }
  
  int get_num_rounds() {

    return clust_costs.size()-1;
  }

  void print_round() {
    assert(clust_costs.size() == num_iter);

    if (num_iter == 0) {
      cout << "No rounds yet" << endl << endl;
      return;
    }

    cout << "---- Round " << num_iter << " ---- " << endl;
    cout << "centroids are at: " << endl;
    for (int i = 0; i < centroids.size(); i++) {
      centroids[i].print_point();
    }

    // c++-0x, OS X does not have this compiler :(
    //    for (auto &cent_i : centroids) {
    //      cent_i.print_point();
    //    }

    cout << "cost is: " << clust_costs[num_iter-1] << endl;
    cout << endl;

  }

  void run() {

    perform_initial_association();
    int prev_cost = clust_costs[clust_costs.size()-1];
    perform_round();
    int curr_cost = clust_costs[clust_costs.size()-1];
    while (curr_cost < prev_cost - round_delta) {
      prev_cost = curr_cost;
      perform_round();
      curr_cost = clust_costs[clust_costs.size()-1];
    }
  }


  void perform_initial_association() {

    init_centroids();
    associate_pts();
    //    print_round();
  }


  void perform_round() {

    recompute_centroids();
    associate_pts();
    //    print_round();
  }




  void gen_unif_rnd_pts(int num_pts_a, int num_dim_a, int rng_seed_a) {

    num_pts = num_pts_a;
    num_dim = num_dim_a;
    rng_seed = rng_seed_a;

    srand(rng_seed);
    
    for (int i = 0; i < num_pts; i++) {
      pts.push_back(gen_unif_rnd_pt());
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

    for (int pt_i = 0; pt_i < pts.size(); pt_i++) {
      int cent_idx = get_nearest_centroid_idx(pts[pt_i]);
      assoc[pt_i] = cent_idx;
    }

    // C++ 0x
    //    for (auto &pt_i : pts) {
    //      int pt_idx = &pt_i - &pts[0];
    //      int cent_idx = get_nearest_centroid_idx(pt_i);
    //      assoc[pt_idx] = cent_idx;
    //    }

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
    //    for (auto &i : sums){i = 0;} // c++-0x
    for (int i = 0; i < sums.size(); i++){sums[i] = 0;}

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

    for (int idx = 0; idx < centroids.size(); idx++){
      double dist = compute_distance(pt, centroids[idx]);
      if (dist < min_dist) {
	cent_idx = idx;
	min_dist = dist;
      }
    }

    // c++ 0x
    //    for (auto &cent_i : centroids){
    //      auto idx = &cent_i - &centroids[0];
    //      double dist = compute_distance(pt, cent_i);
    //      if (dist < min_dist) {
    //	cent_idx = idx;
    //	min_dist = dist;
    //      }
    //    }

    clust_costs[num_iter] += min_dist;
    
    return cent_idx;
  }

  Point gen_bimodal_gauss_pt() {
    vector<double> vals;
    for (int i = 0; i < num_dim; i++) {
      vals.push_back(normal());
    }
    
    Point pt(vals);

    return pt;
  }


  Point gen_unif_rnd_pt() {
    vector<double> vals;
    for (int i = 0; i < num_dim; i++) {
      vals.push_back(rand() % max_pt_val);
    }
    
    Point pt(vals);

    return pt;
  }

};



struct Result{
  int rng_seed;
  int num_pts;
  int num_dim;
  int k_centroids;
  int num_rounds;
  double final_cost;
};

int main(int argc, char* argv[]) {

  int k_centroids, num_dim, num_pts, rng_seed, num_runs;
  double round_delta;
  
  parse_args(argc, argv, &num_pts, &num_dim, &round_delta, &k_centroids, &rng_seed);

  num_runs = 10;
  
  vector<Result> results;

  ClusteringDriver *cd;  

  for (int k_centroids_i = k_centroids;  k_centroids_i < k_centroids + num_runs; k_centroids_i++) {
  
    cd = new ClusteringDriver(k_centroids_i, round_delta);

    // generate pts (uniform) randomly
    cd->gen_rnd_pts(num_pts, num_dim, rng_seed);
    
    // runs until completion
    cd->run();
    Result res;
    res.num_rounds = cd->get_num_rounds();
    res.final_cost = cd->get_final_cost();
    res.num_pts = num_pts;
    res.num_dim = num_dim;
    res.k_centroids = k_centroids_i;
    res.rng_seed = rng_seed;
    
    results.push_back(res);
    cout << "------" << endl;
    cout << "final cost " << res.final_cost << endl;
    cout << "num centroids " << res.k_centroids << endl;
    cout << "num rounds " << res.num_rounds << endl;
    cout << "------" << endl << endl;

    delete cd;
  }

  

  return 0;
}


/**
 * handles arguments in long form
 */
static struct option longopts [] =
  {
    { "help",          no_argument,       NULL, 'h' },
    { "num-points",     required_argument, NULL, 'p' },
    { "num-dimensions",   required_argument, NULL, 'd' },
    { "round-delta",   required_argument, NULL, 'r' },
    { "num-clusters",   required_argument, NULL, 'k' },
    { "rng-seed",   required_argument, NULL, 's' }
  };

static void usage(){
  printf("usage: %s -p <int> -d <int> -k <int> -r <double> -s <int>\n", progname);
}

/**
 * Parses arguments for frequency and number of queries
 */
static void parse_args (int argc, char *argv[], int *num_points, int *num_dimensions, double *round_delta, int *num_clusters, int *rng_seed) {
  progname = argv[0];
  int opt;
  *num_points = 0;
  *num_dimensions = 0;
  *round_delta = 0;
  *num_clusters = 0;
  *rng_seed = 0;
  while(opt = getopt_long(argc, argv, "h:p:d:k:s:r:", 
			  longopts, (int*)NULL), opt != EOF) {
    switch(opt) {
    case 'h':
      usage();
      break;
    case 'p':
      *num_points = atoi(optarg);
      if(*num_points <= 0) usage();
      break;
    case 'd':
      *num_dimensions = atoi(optarg);
      if(*num_dimensions <= 0) usage();
      break;
    case 'r':
      *round_delta = atof(optarg);
      if(*round_delta <= 0) usage();
      break;
    case 'k':
      *num_clusters = atoi(optarg);
      if(*num_clusters <= 0) usage();
      break;
    case 's':
      *rng_seed = atoi(optarg);
      if(*rng_seed < 0) usage();
      break;

    default:
      usage();
      break;
    }
  }


  if(*num_points <= 0) {
    *num_points = 100; 
  }
  if(*num_dimensions <= 0) {
    *num_dimensions = 2;
  }
  if(*round_delta <= 0) {
    *round_delta = 0.00001;
  }
  if(*num_clusters <= 0) {
    *num_clusters = 2;
  }
  if(*rng_seed < 0) {
    *rng_seed = 0;
  }
}
