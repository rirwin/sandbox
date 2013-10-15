
#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <pthread.h>
#include <math.h>

#include <iostream> // for debug
//using namespace std; // for debug
//using namespace mysqlpp; // for debug

#include <ldns/ldns.h>
#include <mysql++.h>

// Parsing/usage functions
char *progname;
static void usage(int s);
static void parse_args(int, char**,int*);

// MYSQL functions
int establish_mysql_database(std::string dbname_str, mysqlpp::Connection &con);
int establish_mysql_tables(std::string rec_arr[], int len, mysqlpp::Connection &con);
int record_query_time(std::string rec_tab_name, int query_time_ms, double curr_mean, double curr_std_dev, mysqlpp::Connection &con);

// DNS query function
uint32_t dns_query_domain_name(ldns_rdf *domain);
void* dns_query_domain_name_function(void *arg);
ldns_rdf* construct_domain_query(std::string dom_str);

int main(int argc, char *argv[]) {

  std::string domain_strings[10] = {"google.com", "facebook.com", "youtube.com", "yahoo.com", "live.com", "wikipedia.org", "baidu.com", "blogger.com", "msn.com", "qq.com"};

  // TODO transform above, consider using a container
  std::string records_tables_strings[10] = {"records_google_com", "records_facebook_com", "records_youtube_com", "records_yahoo_com", "records_live_com", "records_wikipedia_org", "records_baidu_com", "records_blogger_com", "records_msn_com", "records_qq_com"};
 
  pthread_t dns_query_threads[10];

  int freq_sec = 0;
  int dns_query_num = 1; // length should match record_id int length, db starts at 1
  double curr_mean_dns = 0;
  double curr_std_dev_dns = 0; 
  
  // used for mean, std_dev stats
  double old_m[10], old_s[10], new_m[10], new_s[10]; 

  mysqlpp::Connection con;
  std::string dbname_string = "thousandeyes";

  parse_args(argc, argv, &freq_sec);
  printf("frequency is %d seconds \n", freq_sec);

  // (create and connect) or (connect) 
  establish_mysql_database(dbname_string, con);

  // drop and create tables with schema
  establish_mysql_tables(records_tables_strings, 10, con);
	
  // TODO each query loop, create graceful quit, for now ctrl + c
  while (true) { 

    // convert prefix to string for
    std::ostringstream convert; 
    convert << dns_query_num;
    std::string prefix = convert.str();
    
    // launch threads to query each domain
    for (int domain_idx = 0; domain_idx < 10; domain_idx++) {
      
      std::string *dom_str_with_prefix = new std::string(prefix + "." + domain_strings[domain_idx]);
      pthread_create(&dns_query_threads[domain_idx], NULL, dns_query_domain_name_function, (void *) dom_str_with_prefix); 

    } 
    int max_query_time_round = 0;

    for (int domain_idx = 0; domain_idx < 10; domain_idx++) {
      uint32_t query_time_ms = 0;
      void *status;
      pthread_join(dns_query_threads[domain_idx], &status);
      query_time_ms = (uint32_t) status;
      printf("Joined thread %d, query time: %d ms\n", domain_idx, query_time_ms);
     
      int i = domain_idx;
      double x = (double)query_time_ms;

      if (dns_query_num == 1) { 
	old_m[i] = x;
	new_m[i] = x;
	old_s[i] = 0;

	curr_mean_dns = x;
	curr_std_dev_dns = 0;
      }
      else {
	new_m[i] = old_m[i] + (x - old_m[i])/dns_query_num;
	new_s[i] = old_s[i] + (x - old_m[i])*(x - new_m[i]);
	old_m[i] = new_m[i];
	old_s[i] = new_s[i];

	curr_mean_dns = new_m[i];
	curr_std_dev_dns = sqrt(new_s[i]/(dns_query_num-1));
      }

      // record data - TODO include in thread and r/w locking if necessary
      record_query_time(records_tables_strings[domain_idx], query_time_ms, curr_mean_dns, curr_std_dev_dns, con);
      
      // some queries may take up considerable time wrt sleep period
      if (query_time_ms > max_query_time_round) {
	max_query_time_round = query_time_ms;
      }
    }  

    dns_query_num++;

    // could subtract max query time from this, but still a problem because
    // query could take longer than the period length, ignore for now
    if (freq_sec - (double)max_query_time_round/1000 > 0) {
      sleep(freq_sec - (double)max_query_time_round/1000); 
    }
  }  
  
  return 0;
}

int record_query_time(std::string rec_tab_name, int query_time_ms, double curr_mean, double curr_std_dev, mysqlpp::Connection &con) {

  mysqlpp::Query query = con.query();
  try{
    query << "INSERT INTO " << rec_tab_name 
	  <<"(query_time_ms, curr_mean, curr_std_dev) VALUES (" 
	  << query_time_ms <<"," << curr_mean << "," << curr_std_dev << ")";
    query.execute();
  }   
  catch (const mysqlpp::BadQuery& er) {
    // Handle any query errors
    std::cerr << std::endl << "Query error: " << er.what() << std::endl;
    return 1;
  }
  catch (const mysqlpp::BadConversion& er) {
    // Handle bad conversions
    std::cerr << std::endl << "Conversion error: " << er.what() << std::endl <<
      "\tretrieved data size: " << er.retrieved <<
      ", actual size: " << er.actual_size << std::endl;
    return 1;
  }
  catch (const mysqlpp::Exception& er) {
    // Catch-all for any other MySQL++ exceptions
    std::cerr << std::endl << "Error: " << er.what() << std::endl;
    return 1;
  }

  return 0;
}

/**
 * drops existing tables and creates new tables and defines schema
 */
int establish_mysql_tables(std::string rec_arr[], int len, mysqlpp::Connection &con){

  mysqlpp::Query query = con.query();
 
  // create tables
  for (int table_idx = 0; table_idx < 10; table_idx++) {

    try {
  
      // drop records and stats table if exists
      query.exec("DROP TABLE IF EXISTS " + rec_arr[table_idx]);

      // record for each query
      query << "CREATE TABLE " << rec_arr[table_idx]
	    << " ( " 
	    << "entry_id INT UNSIGNED AUTO_INCREMENT," 
	    << "time_stamp TIMESTAMP,"
	    << "query_time_ms SMALLINT UNSIGNED,"
	    << "curr_mean DECIMAL(8,2),"
	    << "curr_std_dev DECIMAL(7,3),"
	    << "PRIMARY KEY (entry_ID)"
	    << " )";
      query.execute();
     
    }   
    catch (const mysqlpp::BadQuery& er) {
      // Handle any query errors
      std::cerr << std::endl << "Query error: " << er.what() << std::endl;
      return 1;
    }
    catch (const mysqlpp::BadConversion& er) {
      // Handle bad conversions
      std::cerr << std::endl << "Conversion error: " << er.what() << std::endl <<
	"\tretrieved data size: " << er.retrieved <<
	", actual size: " << er.actual_size << std::endl;
      return 1;
    }
    catch (const mysqlpp::Exception& er) {
      // Catch-all for any other MySQL++ exceptions
      std::cerr << std::endl << "Error: " << er.what() << std::endl;
      return 1;
    }
  }
  return 0;
}

/**
 * creates or selects db
 */
int establish_mysql_database(std::string dbname_string, mysqlpp::Connection &con) {

   // Connect to database server 
  try {  
    con.connect(0,"localhost","","");  
  }  catch (std::exception& er) {
    std::cerr << "Connection failed: " << er.what() << std::endl;
    return 1;
  }
  
  bool new_db = false;
  
  // no exceptions in this function
  mysqlpp::NoExceptions ne(con); 
  mysqlpp::Query query = con.query();
       
  if (con.select_db(dbname_string)) {
    std::cout << "db " << dbname_string << " found" << std::endl;
  }
  else {
    if (con.create_db(dbname_string) && 
	con.select_db(dbname_string)) {
      new_db = true;
      std::cout << "db " << dbname_string << " not found, creating it" << std::endl;
    }
    else {
      std::cerr << "Error creating DB: " << con.error() << std::endl;
      return 1;
    }
  }
  return 0;
}



ldns_rdf* construct_domain_query(std::string dom_str) {
  char *dom_ch_buff = new char[dom_str.size()+1];
  dom_ch_buff[dom_str.size()] = '\0';
  memcpy(dom_ch_buff, dom_str.c_str(), dom_str.size());
  return ldns_dname_new_frm_str(dom_ch_buff);
}


void* dns_query_domain_name_function(void *arg) { //TODO rename function

  std::string* dom_str_with_prefix = reinterpret_cast<std::string*>(arg);

  ldns_rdf *domain; 
  domain = construct_domain_query(*dom_str_with_prefix);

  ldns_resolver *res;
  ldns_status status = ldns_resolver_new_frm_file(&res, NULL);
  uint32_t query_time;

  if (status != LDNS_STATUS_OK) {
    printf ("cannot create ldns_resolver\n");
    exit(EXIT_FAILURE);
  }
 
  ldns_pkt *p = ldns_pkt_new();		  
  status = ldns_resolver_send(&p,
			 res,
			 domain,
			 LDNS_RR_TYPE_A,
			 LDNS_RR_CLASS_IN,
			 LDNS_RD);
  if (!p || status != LDNS_STATUS_OK)  { 
    query_time = -1;
    printf ("query failure\n");
  } else {    
    //ldns_pkt_print(stdout, p);
    query_time = ldns_pkt_querytime(p);
    printf("%d ms response time\n", query_time);
  }

  ldns_pkt_free(p);
  ldns_resolver_deep_free(res);
  delete domain;
  return (int*)query_time;
}

// Non threaded query domain function
uint32_t dns_query_domain_name(ldns_rdf *domain) {

  ldns_resolver *res;
  ldns_status status = ldns_resolver_new_frm_file(&res, NULL);
  uint32_t query_time;

  if (status != LDNS_STATUS_OK) {
    exit(EXIT_FAILURE);
  }
 
  ldns_pkt *p = ldns_pkt_new();		  
  status = ldns_resolver_send(&p,
			 res,
			 domain,
			 LDNS_RR_TYPE_A,
			 LDNS_RR_CLASS_IN,
			 LDNS_RD);
  if (!p || status != LDNS_STATUS_OK)  { 
    query_time = -1;
  } else {
    
    ldns_pkt_print(stdout, p);
    query_time = ldns_pkt_querytime(p);
    printf("%d ms response time\n", query_time);
  }

  ldns_pkt_free(p);
  ldns_resolver_deep_free(res);
  return query_time;
}

static struct option longopts [] =
  {
    { "help",        no_argument,       NULL, 'h' },
    { "frequency",   required_argument, NULL, 'f' }
  };

static void usage(int s){
  printf("usage: %s -f <int>\n",progname);
  printf("-f <int> the frequency with which to query, (default is 1 sec) \n");
  printf("-h|? this help message\n");

}

static void parse_args (int argc, char *argv[], int *freq_sec) {
  progname = argv[0];
  int opt;
  *freq_sec = 0;
  while(opt = getopt_long(argc, argv, "hf:", 
			  longopts, (int*)NULL), opt != EOF) {
    switch(opt) {
    case 'h':
      usage(0);
      break;
    case 'f':
      *freq_sec = atoi(optarg);
      if(*freq_sec <= 0) usage(-1);
      break;
    default:
      usage(-1);
      break;
    }
  }
  if(*freq_sec <= 0) {
    usage(-1);
    *freq_sec = 1; 
  }
}
