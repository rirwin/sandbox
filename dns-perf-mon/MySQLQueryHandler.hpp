#include <mysql++.h>
#include <string>
#include <vector>
#include <math.h>
#include <pthread.h>

#ifndef _MYSQLQUERYHANDLER_HPP
#define _MYSQLQUERYHANDLER_HPP
class MySQLQueryHandler {

private:

  /**
   * contains the domain name strings
   */
  std::vector<std::string> dom_str_vec;

  /**
   *contains the mysql table names for domains 
   * note '.' cannot appear in a mysql table name
   */
  std::vector<std::string> tbl_str_vec;

  /**
   * holds the connection to the database
   */
  mysqlpp::Connection con;

  /**
   * number of domains to query
   */
  int num_domains;

  /**
   * name of the database
   */
  std::string dbname_string;

  /**
   * Variables used for mean, std_dev stats realtime
   * 1962 paper by B. P. Welford and is presented in 
   * Donald Knuth's Art of Computer Programming, 
   * Vol 2, page 232, 3rd edition. 
   */
  double *old_m, *old_s, *new_m, *new_s; 

  /**
   * array to store the number of queries per domain
   */
  int *dns_query_num;

  /**
   * Used to avoid looping through tbl name vector for index
   * Necessary to find the buffer of previous mean, variance
   */
  std::map<std::string, int> tbl_str_to_idx_map;

  /**
   * Lock for queries to the database
   * Could use c++11 style locks
   */
  pthread_mutex_t mysql_access_lock;

public:

  /**
   * Constructor 
   */
  MySQLQueryHandler(std::string dbname_string_a, std::vector<std::string> dom_str_vec_a){

    dom_str_vec = dom_str_vec_a;
    num_domains = dom_str_vec.size();

    int idx = 0;
    for (std::vector<std::string>::iterator it = dom_str_vec.begin() ; it != dom_str_vec.end(); ++it) {
      std::string tbl_name = transform_dom_str(*it); 
      tbl_str_vec.push_back(tbl_name);
      tbl_str_to_idx_map.insert(std::pair<std::string,int> (tbl_name, idx++));
    }

    dbname_string = dbname_string_a;

    if (create_connection() > 0){
      std::cout << "MySQLQueryHandler Constructor failed to create connection" << std::endl;
      return;
    }

    if (establish_database() > 0) {
      std::cout << "MySQLQueryHandler Constructor failed to establish database" << std::endl;
      return;
    }
 
    if (establish_tables() > 0) {
      std::cout << "MySQLQueryHandler Constructor failed to establish tables" << std::endl;
      return;
    }

    old_m = (double*)malloc(num_domains*sizeof(double));
    old_s = (double*)malloc(num_domains*sizeof(double));
    new_m = (double*)malloc(num_domains*sizeof(double));
    new_s = (double*)malloc(num_domains*sizeof(double));

    dns_query_num = (int*)malloc(num_domains*sizeof(int));
    for (int i = 0; i < num_domains; i++)
      dns_query_num[i] = 1;
  }

  /**
   * De-constructor 
   */
  ~MySQLQueryHandler() {

    free(old_m);
    free(old_s);
    free(new_m);
    free(new_s);
    free(dns_query_num);
  }

  /**
   * Public handle for accepting another entry
   * Calculate the mean and standard devation 
   */
  int process_dns_query_time(std::string dom_name, int query_time_ms) {

    // table names are private
    std::string tbl_name = transform_dom_str(dom_name);

    int i = tbl_str_to_idx_map.find(tbl_name)->second;

    // Calculate the mean and standard devation  
    double x = (double)query_time_ms;
    double curr_mean, curr_std_dev;

    if (dns_query_num[i] == 1) { 
      old_m[i] = x;
      new_m[i] = x;
      old_s[i] = 0;

      curr_mean = x;
      curr_std_dev = 0;
    }
    else {
      new_m[i] = old_m[i] + (x - old_m[i])/dns_query_num[i];
      new_s[i] = old_s[i] + (x - old_m[i])*(x - new_m[i]);
      old_m[i] = new_m[i];
      old_s[i] = new_s[i];

      curr_mean = new_m[i];
      curr_std_dev = sqrt(new_s[i]/(dns_query_num[i]-1));
    }


    pthread_mutex_lock (&mysql_access_lock); 
    record_query_stats(tbl_name, query_time_ms, curr_mean, curr_std_dev);
    pthread_mutex_unlock (&mysql_access_lock);

    dns_query_num[i] = dns_query_num[i] + 1;

    return 0;
  }

  /**
   * Prints stats summaries for all domains thus far.
   * All entries contain a running mean and standard deviation 
   * to ease computation burden at the end.
   */
  void print_all_stats_summaries() {

    for (int dom_idx = 0; dom_idx < num_domains; dom_idx++) {

       pthread_mutex_lock (&mysql_access_lock); 
       print_stat_summary_for_tbl(dom_idx); 
       pthread_mutex_unlock (&mysql_access_lock); 
     }

  }


private:

  /**
   * transform google.com to google_com for mysql table name
   * returns a new string
   */
  std::string transform_dom_str(std::string str) {
    size_t i = str.find_last_of('.');
    str[i] = '_';
    return str;
  }

  /**
   * Initializes a connection 
   */
  int create_connection() {
    try {  
      con.connect(0,"localhost","","");  
    }  
    catch (std::exception& er) {
      std::cerr << "Connection failed: " << er.what() << std::endl;
      return 1;
    }
    return 0;
  }

  /**
   * Creates a database with dbname_string if one doesn't exist
   */
  int establish_database() {
      
    // handles exceptions in this function
    mysqlpp::NoExceptions ne(con); 
    mysqlpp::Query query = con.query();
       
    if (con.select_db(dbname_string)) {
      std::cout << "db " << dbname_string << " found" << std::endl;
    }
    else {
      if (con.create_db(dbname_string) && 
	  con.select_db(dbname_string)) {
	std::cout << "db " << dbname_string << " not found, creating it" << std::endl;
      }
      else {
	std::cerr << "Error creating DB: " << con.error() << std::endl;
	return 1;
      }
    }
    return 0;
  }

  /**
   * Every domain perfomance monitor gets its own table.
   * Could be a single table.
   * Drops any existing tables with names of those being created.
   * Establishes schema.
   */  
  int establish_tables() {
  
    mysqlpp::Query query = con.query();
 
    // create tables
    for (std::vector<std::string>::iterator it = tbl_str_vec.begin() ; it != tbl_str_vec.end(); ++it) {

      try {
	
	// drop records and stats table if exists
	query.exec("DROP TABLE IF EXISTS " + *it);

	// record for each query
	query << "CREATE TABLE " << *it
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
   * Records query times
   */
  int record_query_stats(std::string tbl_name, int query_time_ms, double curr_mean, double curr_std_dev){
    
    mysqlpp::Query query = con.query();
    try{
      query << "INSERT INTO " << tbl_name 
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
   * Prints the stat summary of table
   */
  int print_stat_summary_for_tbl(int dom_idx) {
    
    std::string tbl_name = tbl_str_vec[dom_idx];
    std::string dom_name = dom_str_vec[dom_idx];

    try{
      mysqlpp::Query query1 = con.query();
      query1 << "SELECT COUNT(*) count FROM " << tbl_name;
      
      query1.parse();
      mysqlpp::StoreQueryResult res_count = query1.store();
      
      mysqlpp::Query query2 = con.query();
      query2 << " SELECT entry_id, time_stamp, curr_mean, curr_std_dev "
	     << " FROM " << tbl_name 
	     << " WHERE entry_id >= ("
	     << "   SELECT MAX(entry_id) "
	     << "   FROM " << tbl_name << ")";

      query2.parse();
      mysqlpp::StoreQueryResult res_curr_stats = query2.store();

      mysqlpp::Query query3 = con.query();
      query3 << " SELECT time_stamp FROM " << tbl_name
	     << " WHERE time_stamp <= ("
	     << "   SELECT TIMESTAMP(MIN(time_stamp)) "
	     << "   FROM " << tbl_name << ")";

      query3.parse();
      mysqlpp::StoreQueryResult res_earliest_timestamp = query3.store();

      if (res_count && res_curr_stats && res_earliest_timestamp) {

	// nice looking dividers with '='
	for (int i = 0; i < 14; i++) std::cout <<"=";
	std::cout << " " << dom_name << " ";
	for (int i = 0; i < 28 - dom_name.length(); i++) std::cout <<"=";
	std::cout << std::endl;

	std::cout << "  Total Queries : " << res_count[0]["count"] << std::endl;
	std::cout << "  Earliest Timestamp : " << res_earliest_timestamp[0]["time_stamp"] << std::endl;
	std::cout << "  Latest Timestamp   : " << res_curr_stats[0]["time_stamp"] << std::endl;
	std::cout << "  Mean DNS Query Time (ms): " << res_curr_stats[0]["curr_mean"] << std::endl;
	std::cout << "  Standard Deviation      : " << res_curr_stats[0]["curr_std_dev"] << std::endl;

	// same text width as '=' divider above
	for (int i = 0; i < 44; i++) std::cout <<"=";
	std::cout << std::endl << std::endl;
	}
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
  
};
#endif
