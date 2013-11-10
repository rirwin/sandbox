#include <iostream>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h> 
#include <math.h>
#include <cfloat>
#include <getopt.h>
#include "ClusteringHandler.hpp"
#include "Point.hpp"


using namespace std;

/**
 * Parsing/usage functions
 */
char *progname;
static void usage();
static void parse_args (int argc, char *argv[], int *num_points, int *num_dimensions, double *round_delta, int *num_clusters, int *rng_seed);


int main(int argc, char* argv[]) {

  int k_centroids, num_dim, num_pts, rng_seed, num_runs;
  double round_delta;
  
  parse_args(argc, argv, &num_pts, &num_dim, &round_delta, &k_centroids, &rng_seed);
 

  ClusteringHandler *pch;  
  
  pch = new ClusteringHandler(k_centroids, round_delta, rng_seed);

  // generate pts (uniform) randomly
  pch->gen_bimodal_gauss_rnd_indep_dim_pts(num_pts, num_dim);

  // runs until completion
  pch->run();

  pch->print_round();
  
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
