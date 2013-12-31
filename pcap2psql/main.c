#include <stdio.h>
#include <stdlib.h>
#include <pcap.h>
#include "psql_writer.h"
#include "pcap_reader.h"

PGconn *conn;
const char * table;

int main(int argc, char **argv) 
{

  pcap_t *fp;
  char errbuf[PCAP_ERRBUF_SIZE];
  char *dbname;
  
  char conninfo[256];
  const char *schema;

  PGresult   *res;
  int         nFields;
  int         i,j;
  int accum_size;

  if (argc != 5) {
    printf("usage: %s filename dbname tablename numrowaccum", argv[0]);
    return -1;
  }

  accum_size = atoi(argv[4]);

  // construct string "dbname = <val of argv[2]>"
  dbname = argv[2];
  sprintf(conninfo,"dbname = %s",dbname);

  // table name
  table = argv[3];
 
  // Hard coded schema copied from MySQLReader.java from Alex
  schema = "(cap_time int8, pkt_len int4, cap_len int4, src_ip cidr, dest_ip cidr, protocol int4, src_port int4, dest_port int4, src_mac macaddr, dest_mac macaddr)"; 

  // initialize connection to database
  conn = init_conn(conninfo);

  // drops table if exists
  drop_table(conn, table);

  // creates table if not exists
  create_table(conn, table, schema);

  // initialize packet capture file
  fp = init_pcap(argv[1]);

  if (fp) {
    // main loop for reading capture file and writing to db
    // find a way to separate packet capture from database 
    capture_loop(fp, 0, conn, table, accum_size); 
  }
  else {
    return -1;
  }

  return 0;

}
