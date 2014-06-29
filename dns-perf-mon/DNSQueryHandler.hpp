#include <ldns/ldns.h>
#include <mysql++.h>
#include <string>
#include <iostream>

#include "MySQLQueryHandler.hpp"

#ifndef _DNSQUERYHANDLER_HPP
#define _DNSQUERYHANDLER_HPP
class DNSQueryHandler {
 
private:

  /**
   * Domain string of this DNSQueryHandler
   */
  std::string dom_str;

  /**
   * contains full domain
   */
  ldns_rdf *full_domain; 

  /**
   * Number of queries executed for this domain
   */
  int num_queries_performed;

  /**
   * Time interval in seconds between queries
   */
  int query_freq_ms;

  /**
   * Number of queries to perform
   */
  int num_queries_to_perform;

  /**
   * ldns status variable
   */
  ldns_status status;

  /**
   * MySQLQueryHandler for recording stats of query
   */
  MySQLQueryHandler *mysql_handler;

public:
  DNSQueryHandler(std::string dom_str_a, int query_freq_ms_a, int num_queries_to_perform_a, MySQLQueryHandler *mysql_handler_a) {

    dom_str = dom_str_a;
    query_freq_ms = query_freq_ms_a;
    num_queries_to_perform = num_queries_to_perform_a;
    mysql_handler = mysql_handler_a;

    num_queries_performed = 0;
  }

  ~DNSQueryHandler() {

  }

  std::string get_dom_str() {
    return dom_str;
  }

  int get_query_freq_ms() {
    return query_freq_ms;
  }

  int get_num_queries_to_perform() {
    return num_queries_to_perform;
  }

  int get_num_queries_performed() {
    return num_queries_performed;
  }

  int perform_query() { 

    uint32_t query_time;

    // avoid DNS cache by adding a prefix to domain
    std::ostringstream convert; 
    convert << ++num_queries_performed;
    std::string prefix = convert.str();
    std::string full_dom_str = prefix + "." + dom_str;
   
    // ldns_resolver needed
    ldns_resolver *res;   
    status = ldns_resolver_new_frm_file(&res, NULL);
    if (status != LDNS_STATUS_OK) {
      std::cout << "cannot create ldns_resolver\n";
      exit(EXIT_FAILURE);
    }

    // construct domain object
    full_domain = construct_domain_query(full_dom_str);

    ldns_pkt *p = ldns_pkt_new();		  
    status = ldns_resolver_send(&p,
				res,
				full_domain,
				LDNS_RR_TYPE_A,
				LDNS_RR_CLASS_IN,
				LDNS_RD);


    if (!p || status != LDNS_STATUS_OK)  { 
      query_time = -1;
      std::cout << "query failure " << full_dom_str << std::endl;
    } 
    else {    
      //ldns_pkt_print(stdout, p);
      query_time = ldns_pkt_querytime(p);
      std::cout << dom_str << " "<< query_time << " ms response time" << std::endl;
    }

    // Free items
    ldns_resolver_deep_free(res); 
    ldns_pkt_free(p); 
    ldns_rdf_deep_free(full_domain);
    
    mysql_handler->process_dns_query_time(dom_str, query_time);

    return (int)query_time;
  }

private:

  /**
   * convert string to char array to create ldns_rdf object
   */
  ldns_rdf* construct_domain_query(std::string dom_str) {

    char *dom_ch_buff = new char[dom_str.size()+1];
    dom_ch_buff[dom_str.size()] = '\0';
    memcpy(dom_ch_buff, dom_str.c_str(), dom_str.size());

    return ldns_dname_new_frm_str(dom_ch_buff);
  }


};
#endif
