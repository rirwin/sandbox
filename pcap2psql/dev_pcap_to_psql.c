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
#include <libpq-fe.h>

#define LINE_LEN 16

static void
exit_nicely(PGconn *conn)
{
    PQfinish(conn);
    exit(1);
}

// global var for link layer header (TODO see if this can be removed)
int linkhdrlen;
PGconn *conn;


// structure for containing result of individual packet capture
struct packet_filter
{
  double cap_time;
  int pkt_len, cap_len, src_port, dest_port, protocol;
  char src_ip[256], dest_ip[256], src_mac[256], dest_mac[256];
}__attribute__((packed));

void dispatcher_handler(u_char *, const struct pcap_pkthdr *, const u_char * );
void print_packet(const struct pcap_pkthdr *header, const u_char *pkt_data);

void parse_packet(u_char *user, struct pcap_pkthdr *packethdr, u_char *packetptr);
void capture_loop(pcap_t* pd, int packets, pcap_handler func);


int main(int argc, char **argv) {

  pcap_t *fp;
  char errbuf[PCAP_ERRBUF_SIZE];

  const char *conninfo;

  PGresult   *res;
  int         nFields;
  int         i,j;

  /*
   * If the user supplies a parameter on the command line, use it as the
   * conninfo string; otherwise default to setting dbname=postgres and using
   * environment variables or defaults for all other connection parameters.
   */
  conninfo = "dbname = rirwin";

  /* Make a connection to the database */
  conn = PQconnectdb(conninfo);

  /* Check to see that the backend connection was successfully made */
  if (PQstatus(conn) != CONNECTION_OK)
    {
      fprintf(stderr, "Connection to database failed: %s",
	      PQerrorMessage(conn));
      exit_nicely(conn);
    }

  /* Start a transaction block */
  res = PQexec(conn, "DROP TABLE IF EXISTS test1");
  if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
      fprintf(stderr, "DROP TABLE IF EXISTS test1 command failed: %s", PQerrorMessage(conn));
      PQclear(res);
      exit_nicely(conn);
    }
 
  PQclear(res);

  res = PQexec(conn, "CREATE TABLE if not exists test1 (cap_time float8, pkt_len int4, cap_len int4, src_ip cidr, dest_ip cidr, protocol int4, src_port int4, dest_port int4, src_mac macaddr, dest_mac macaddr);");
  if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        fprintf(stderr, "CREATE TABLE if not exists test1 failed: %s", PQerrorMessage(conn));
        PQclear(res);
        exit_nicely(conn);
    }
    PQclear(res);




  if (argc != 2) {
    printf("usage: %s filename", argv[0]);
    return -1;
  }

  if ( (fp = pcap_open_offline(argv[1], errbuf) ) == NULL) {
    fprintf(stderr, "\nError opening dump file\n");
    return -1;
  }

  /* Open a capture file */
  if ( (fp = pcap_open_offline(argv[1], errbuf) ) == NULL) {
    fprintf(stderr, "\nError opening dump file\n");
    return -1;
  }

  //pcap_loop(fp, 0, dispatcher_handler, NULL);
  //pcap_loop(fp, 0, (pcap_handler)parse_packet, NULL);
  capture_loop(fp, 0, (pcap_handler)parse_packet);

  return 0;

}


void dispatcher_handler(u_char *temp1, const struct pcap_pkthdr *header, const u_char *pkt_data)
{
  u_int i = 0;

  printf("dispatching packet\n");

  /* save packet to dump file */
  print_packet(header, pkt_data);
  /*pcap_dump(dumpfile, header, pkt_data);
  
  pkts_capped++;
  if (pkts_capped >= pkts_to_cap) {
    pcap_dump_close(dumpfile);
    exit(0);
    }*/

}

void print_packet(const struct pcap_pkthdr *header, const u_char *pkt_data)
{
  int i;
  /* Print pkt timestamp and length */
  printf("%ld:%ld (%d)\n", header->ts.tv_sec, header->ts.tv_usec, header->len);
  /*
    schema from MySQLReader.java
    process UDP packet (TCP looks the same)
    (capture_date_time, packet_len, capture_len, source_ip, dest_ip, 
     protocol, source_port, dest_port, source_mac, dest_mac, packet)

     Date date = new java.sql.Date(packet.sec * 1000 + packet.usec / 1000);
     Timestamp dateTime = new Timestamp(date.getTime());
  */

  struct ip* iphdr;
  struct icmphdr* icmphdr;
  struct tcphdr* tcphdr;
  struct udphdr* udphdr;
  char iphdrInfo[256], srcip[256], dstip[256];
  unsigned short id, seq;

  
  /* print */
  for (i = 1; (i < header->caplen + 1) ; i++) {
    printf("%.2x ", pkt_data[i-1]);
    if ( (i % LINE_LEN) == 0) 
      printf("\n");
  }
  printf("\n\n");
}

void parse_packet(u_char *user, struct pcap_pkthdr *packethdr, 
                  u_char *packetptr)
{
  struct ip* iphdr;
  struct icmphdr* icmphdr;
  struct tcphdr* tcphdr;
  struct udphdr* udphdr;
  struct ether_header *eptr;
  u_char *ptr;
  char iphdrInfo[256], src_ip[256], dest_ip[256], src_mac[256], dest_mac[256];
  char full_row[1024];
  unsigned short id, seq;
  double cap_time;
  int pkt_len, cap_len, src_port, dest_port, protocol,i;
  PGresult   *res;
  struct ether_header *p = (struct ether_header *)packetptr;

  // duplicated code from MySQLReader.java
  cap_time = 1000*packethdr->ts.tv_sec + (double)packethdr->ts.tv_usec/1000;
  cap_len = packethdr->caplen;
  pkt_len = packethdr->len;

  // sprintf mac addresses
  sprintf(dest_mac,"%02X:%02X:%02X:%02X:%02X:%02X",
	 p->ether_dhost[0], p->ether_dhost[1], p->ether_dhost[2],
	 p->ether_dhost[3], p->ether_dhost[4], p->ether_dhost[5]);

  sprintf(src_mac,"%02X:%02X:%02X:%02X:%02X:%02X",
	 p->ether_shost[0], p->ether_shost[1], p->ether_shost[2],
	 p->ether_shost[3], p->ether_shost[4], p->ether_shost[5]);

  // Done with datalink layer header, go to the IP header fields.
  packetptr += linkhdrlen;
  iphdr = (struct ip*)packetptr;

  // strcpy ip addresses
  strcpy(src_ip, inet_ntoa(iphdr->ip_src));
  strcpy(dest_ip, inet_ntoa(iphdr->ip_dst));
  sprintf(iphdrInfo, "ID:%d TOS:0x%x, TTL:%d IpLen:%d DgLen:%d",
	  ntohs(iphdr->ip_id), iphdr->ip_tos, iphdr->ip_ttl,
	  4*iphdr->ip_hl, ntohs(iphdr->ip_len));
 
  // Advance to the transport layer header then parse and display
  // the fields based on the type of hearder: tcp, udp or icmp.
  packetptr += 4*iphdr->ip_hl;

  // save packet protocol
  protocol = iphdr->ip_p;

  switch (iphdr->ip_p)
    {
    case IPPROTO_TCP:
      tcphdr = (struct tcphdr*)packetptr;

      src_port = ntohs(tcphdr->source);
      dest_port = ntohs(tcphdr->dest);

      printf("TCP  %s:%d -> %s:%d\n", src_ip, ntohs(tcphdr->source),
	     dest_ip, ntohs(tcphdr->dest));

      printf("%s\n", iphdrInfo);
      printf("%c%c%c%c%c%c Seq: 0x%x Ack: 0x%x Win: 0x%x TcpLen: %d\n",
	     (tcphdr->urg ? 'U' : '*'),
	     (tcphdr->ack ? 'A' : '*'),
	     (tcphdr->psh ? 'P' : '*'),
	     (tcphdr->rst ? 'R' : '*'),
	     (tcphdr->syn ? 'S' : '*'),
	     (tcphdr->fin ? 'F' : '*'),
	     ntohl(tcphdr->seq), ntohl(tcphdr->ack_seq),
	     ntohs(tcphdr->window), 4*tcphdr->doff);
      break;
 
    case IPPROTO_UDP:
      udphdr = (struct udphdr*)packetptr;

      src_port = ntohs(udphdr->source);
      dest_port = ntohs(udphdr->dest);

      printf("UDP  %s:%d -> %s:%d\n", src_ip, ntohs(udphdr->source),
	     dest_ip, ntohs(udphdr->dest));

      printf("%s\n", iphdrInfo);
      break;
 
    case IPPROTO_ICMP: 
      icmphdr = (struct icmphdr*)packetptr;
      src_port = -1;
      dest_port = -1;
      printf("ICMP %s -> %s\n", src_ip, dest_ip);
      printf("%s\n", iphdrInfo);
      memcpy(&id, (u_char*)icmphdr+4, 2);
      memcpy(&seq, (u_char*)icmphdr+6, 2);
      printf("Type:%d Code:%d ID:%d Seq:%d\n", icmphdr->type, icmphdr->code, 
	     ntohs(id), ntohs(seq));
      break;
    }

  printf("Capture Time: %lf\n",cap_time);
  printf("Packet Len: %d\n",pkt_len);
  printf("Capture Len: %d\n",cap_len);
  printf("Source IP: %s\n",src_ip);
  printf("Destination IP: %s\n",dest_ip);
  printf("Protocol: %d\n",protocol);
  printf("Source port: %d\n",src_port);
  printf("Destination port: %d\n",dest_port);
  printf("Source MAC addr: %s\n", src_mac);
  printf("Destination MAC addr: %s\n", dest_mac);
  sprintf(full_row, "%lf, %d, %d, %s, %s, %d, %d, %d, %s, %s);",cap_time,pkt_len,cap_len,src_ip, dest_ip, protocol, src_port, dest_port, src_mac, dest_mac);
  sprintf(full_row,"INSERT INTO test1 values (%lf, %d, %d, '%s', '%s', %d, %d, %d, '%s', '%s');",cap_time,pkt_len,cap_len,src_ip, dest_ip, protocol, src_port, dest_port, src_mac, dest_mac);

  printf("%s",full_row);
  res = PQexec(conn, full_row);



  // (%lf, %d, %d, %s, %s, %d, %d, %d, %s, %s, cap_time float8, pkt_len int4, cap_len int4, src_port int4, dest_port int4, protocol int4, src_ip cidr, dest_ip cidr, src_mac macaddr, dest_mac macaddr);");
  if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        fprintf(stderr, "CREATE TABLE if not exists test1 failed: %s", PQerrorMessage(conn));
        PQclear(res);
        exit_nicely(conn);
    }
    PQclear(res);

  printf(
	 "+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");
}

void capture_loop(pcap_t* pd, int packets, pcap_handler func)
{
    int linktype;
 
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
 
    // Start capturing packets.
    if (pcap_loop(pd, packets, func, 0) < 0)
        printf("pcap_loop failed: %s\n", pcap_geterr(pd));
}
