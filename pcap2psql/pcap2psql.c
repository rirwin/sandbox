#include <stdio.h>
#include <pcap.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <netinet/ip_icmp.h>
#include <netinet/if_ether.h>
#include "psql_writer.h"
#include "pcap_helper.h"

// Prints accumulation of row to insert 
const int MAIN_DEBUG = 0;
void capture_loop(pcap_t* pd, int packets, PGconn* conn, const char* table, const int accum_size);

int main(int argc, char **argv) 
{

  PGconn *conn;
  const char * table;
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
    capture_loop(fp, 0, conn, table, accum_size); 
  }
  else {
    return -1;
  }

  return 0;

}


void capture_loop(pcap_t* pd, int packets, PGconn* conn, const char* table, const int accum_size)
{
  int linktype,linkhdrlen=4;
  struct pcap_pkthdr *header;
  const u_char *pkt_data;
  int rows_accumed = 0;
  char **rows;
  char *row;
  int res,i;

  rows = (char**)malloc(accum_size*sizeof(char*));

  // Determine the datalink layer type.
  if ((linktype = pcap_datalink(pd)) < 0)
    {
      printf("pcap_datalink(): %s\n", pcap_geterr(pd));
      return;
    }
 
  // Set the datalink layer header size.
  switch (linktype)
    {
    case DLT_NULL:
      linkhdrlen = 4;
      break;
 
    case DLT_EN10MB:
      linkhdrlen = 14;
      break;
 
    case DLT_SLIP:
    case DLT_PPP:
      linkhdrlen = 24;
      break;
 
    default:
      printf("Unsupported datalink (%d)\n", linktype);
      return;
    }

  // Retrieve the packets from the file 
  while((res = pcap_next_ex( pd, &header, &pkt_data)) >= 0){

    row = parse_packet(header, pkt_data, linkhdrlen);

    if (accum_size > 1) { // TODO for some reason 1 puts dup entries
      rows[rows_accumed] = row;
      rows_accumed++;
      
      if (rows_accumed == accum_size) {
	if (MAIN_DEBUG) {
	  printf ("ROWS DUMP\n\n");
	  for (i = 0; i < rows_accumed; i++)
	    printf("%s\n",rows[i]);
	}
	insert_rows(conn, table, rows, rows_accumed);
	rows_accumed = 0;
      }
    }
    else {
    // single row dump
      insert_row(conn, table, row); rows_accumed = 0;
    }

    if (MAIN_DEBUG) {
      // Print the packet
      for (i=1; (i < header->caplen + 1 ) ; i++) {
	printf("%.2x ", pkt_data[i-1]);
	if ( (i % 16) == 0) printf("\n");
      }
        
      printf("\n\n");     
    }
  }
  
  if (rows_accumed > 0 && accum_size > 1) { // TODO check extra inserts
    if (MAIN_DEBUG) {
      printf ("ROWS DUMP\n\n");
      for (i = 0; i < rows_accumed; i++)
	printf("%s\n",rows[i]);
    }
    
    insert_rows(conn, table, rows, rows_accumed);
    rows_accumed = 0;
  }
  
}

