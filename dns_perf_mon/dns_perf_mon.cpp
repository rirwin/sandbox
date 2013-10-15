#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <string>
#include <pthread.h>
#include <iostream>
#include <vector>

#include "MySQLQueryHandler.hpp"
#include "DNSQueryHandler.hpp"

/**
 * Parsing/usage functions
 */
char *progname;
static void usage();
static void parse_args(int, char**,int*, int*);

/**
 * function for calling dns query threads
 */
void* dns_query_thread_function(void *arg);

/**
 * main
 */
int main(int argc, char *argv[]) {

  // variables for arguments to program
  int freq_ms, num_queries;
  parse_args(argc, argv, &freq_ms, &num_queries);
  printf("Query once every %d ms for a total of %d queries\n", freq_ms, num_queries);

  // vector containing the domain names as strings
  // could use c++11 to make this faster
  std::vector<std::string> dom_str_vec; 
  dom_str_vec.push_back("google.com");
  dom_str_vec.push_back("facebook.com");
  dom_str_vec.push_back("youtube.com");
  dom_str_vec.push_back("yahoo.com");
  dom_str_vec.push_back("live.com");
  dom_str_vec.push_back("wikipedia.org");
  dom_str_vec.push_back("baidu.com");
  dom_str_vec.push_back("blogger.com");
  dom_str_vec.push_back("msn.com");
  dom_str_vec.push_back("qq.com");

  int num_domains = dom_str_vec.size();

  // Initialize database called thousandeyes with a table
  // for each domain to be queried
  MySQLQueryHandler *mysql_handler = new MySQLQueryHandler("thousandeyes",dom_str_vec);

  // A container of DNS query handlers
  std::vector<DNSQueryHandler*> dns_handlers;

  // Initialize container
  for (int dom_idx = 0; dom_idx < num_domains; dom_idx++) {
    DNSQueryHandler *handler = new DNSQueryHandler(dom_str_vec[dom_idx], freq_ms, num_queries, mysql_handler); 
    dns_handlers.push_back(handler);
  }
  
  // threads for handling
  pthread_t dns_query_threads[num_domains];
  for (int domain_idx = 0; domain_idx < num_domains; domain_idx++) {
    pthread_create(&dns_query_threads[domain_idx], NULL, dns_query_thread_function, (void *) dns_handlers[domain_idx]); 
  }

  // Join the threads and check the status
  for (int domain_idx = 0; domain_idx < num_domains; domain_idx++) {

    void *status;
    pthread_join(dns_query_threads[domain_idx], &status); 
    if ((int)status != 0){
      std::cout << "Non-zero status " << (int)status 
		<< " for " << dom_str_vec[domain_idx] << std::endl;
    }
    else {
      std::cout << "Joined thread for " << dom_str_vec[domain_idx] << std::endl;
      std::cout << dns_handlers[domain_idx]->get_num_queries_performed() 
		<< " Queries performed" << std::endl;
    }
  }

  // Print out final stats for all domains
  mysql_handler->print_all_stats_summaries();

  // Free memory    
  for (int i = 0; i < num_domains; i++) {
    delete dns_handlers[i];
  }
  delete mysql_handler;

  return 0;
}

/**
 * dns query thread function passed when creating threads
 */
void* dns_query_thread_function(void *arg) {

  DNSQueryHandler *dns_qh;
  dns_qh = (DNSQueryHandler*) arg;
  
  if (dns_qh) {
    int query_freq_ms = dns_qh->get_query_freq_ms();
    int num_queries_to_perform = dns_qh->get_num_queries_to_perform();
    
    for (int query_i = 0; query_i < num_queries_to_perform; query_i++) {

      std::cout << "launching query " << dns_qh->get_num_queries_performed()+1;
      std::cout << " to " << dns_qh->get_dom_str() << std::endl; 

      int query_time_ms = dns_qh->perform_query();

      // print warning if query time > sleep period (frequency)
      if (query_freq_ms - (double)query_time_ms > 0) {
	usleep(1000*((double)query_freq_ms - (double)query_time_ms)); 
      }
      else {
	std::cout << "WARNING: query response time for "
		  << dns_qh->get_dom_str() 
		  << " exceeded query interval time (frequency)" 
		  << std::endl;
      }

    }
    return (int*)0;
  }

  return (int*)1;
}

/**
 * handles arguments in long form
 */
static struct option longopts [] =
  {
    { "help",          no_argument,       NULL, 'h' },
    { "frequency",     required_argument, NULL, 'f' },
    { "num-queries",   required_argument, NULL, 'n' }
  };

static void usage(){
  printf("usage: %s -f <int> -n <int>\n",progname);
  printf("-f <int> the frequency with which to query, (default is 1000 ms) \n");
  printf("-n <int> the number of times to query (default is 10) \n");
  printf("-h|? this help message\n");

}

/**
 * Parses arguments for frequency and number of queries
 */
static void parse_args (int argc, char *argv[], int *freq_ms, int *num_queries) {
  progname = argv[0];
  int opt;
  *freq_ms = 0;
  *num_queries = 0;
  while(opt = getopt_long(argc, argv, "hf:n:", 
			  longopts, (int*)NULL), opt != EOF) {
    switch(opt) {
    case 'h':
      usage();
      break;
    case 'f':
      *freq_ms = atoi(optarg);
      if(*freq_ms <= 0) usage();
      break;
    case 'n':
      *num_queries = atoi(optarg);
      if(*num_queries <= 0) usage();
      break;
    default:
      usage();
      break;
    }
  }

  if (*freq_ms <= 0 && *num_queries <= 0) {
    usage();
  }

  if(*freq_ms <= 0) {
    *freq_ms = 1000; 
  }
  if(*num_queries <= 0) {
    *num_queries = 10;
  }
}
