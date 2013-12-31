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

// Prints contents of header info, packet contents 
const int PCAP_HELPER_DEBUG = 0;

void parse_packet_insert(struct pcap_pkthdr *packethdr, const u_char *packetptr, PGconn* conn, const char * table, int linkhdrlen);
char* parse_packet(struct pcap_pkthdr *packethdr, const u_char *packetptr, int linkhdrlen);

pcap_t* init_pcap(const char * dump_file){
  
  pcap_t *fp;
  char errbuf[PCAP_ERRBUF_SIZE];

  /* Open a capture file */
  if ( (fp = pcap_open_offline(dump_file, errbuf) ) == NULL) {
    fprintf(stderr, "\nError opening dump file\n");
    return NULL;
  }
  return fp;
}


/**
 * Function for parsing packet, returns a string that matches schema for insert
 */
char* parse_packet(struct pcap_pkthdr *packethdr, const u_char *packetptr, int linkhdrlen)
{
  struct ip* iphdr;
  struct icmphdr* icmphdr;
  struct tcphdr* tcphdr;
  struct udphdr* udphdr;
  struct ether_header *eptr;
  u_char *ptr;
  //TODO reduce the size of these char arrays
  char iphdrInfo[256], src_ip[256], dest_ip[256], src_mac[256], dest_mac[256];
  char* vp; // values pointer
  unsigned short id, seq;
  long long cap_time;  // TODO a String object in yyyy-mm-dd hh:mm:ss.fffffffff format
  int pkt_len, cap_len, src_port, dest_port, protocol,i;
  PGresult   *res;
  struct ether_header *p = (struct ether_header *)packetptr;
  vp = (char*)malloc(256*sizeof(char));


  // gets number of milliseconds since epoch
  // cap_time = 1000*packethdr->ts.tv_sec + (double)packethdr->ts.tv_usec/1000;
  // number of seconds since epoch
  cap_time = packethdr->ts.tv_sec*1000000 + packethdr->ts.tv_usec;

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

  // switch block to get source and dest ports
  switch (iphdr->ip_p)
    {
    case IPPROTO_TCP:
      tcphdr = (struct tcphdr*)packetptr;
      src_port = ntohs(tcphdr->source);
      dest_port = ntohs(tcphdr->dest);

      if (PCAP_HELPER_DEBUG) {
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
      }
      break;
 
    case IPPROTO_UDP:
      udphdr = (struct udphdr*)packetptr;
      src_port = ntohs(udphdr->source);
      dest_port = ntohs(udphdr->dest);

      if (PCAP_HELPER_DEBUG) {
	printf("UDP  %s:%d -> %s:%d\n", src_ip, ntohs(udphdr->source),
	       dest_ip, ntohs(udphdr->dest));

	printf("%s\n", iphdrInfo);
	    }
      break;
 
    case IPPROTO_ICMP: 
      icmphdr = (struct icmphdr*)packetptr;
      src_port = -1;
      dest_port = -1;

      if (PCAP_HELPER_DEBUG) {
	printf("ICMP %s -> %s\n", src_ip, dest_ip);
	printf("%s\n", iphdrInfo);
	memcpy(&id, (u_char*)icmphdr+4, 2);
	memcpy(&seq, (u_char*)icmphdr+6, 2);
	printf("Type:%d Code:%d ID:%d Seq:%d\n", icmphdr->type, icmphdr->code, 
	       ntohs(id), ntohs(seq));
      }
      break;
    }

  sprintf(vp,"(%lld, %d, %d, '%s', '%s', %d, %d, %d, '%s', '%s')",cap_time,pkt_len,cap_len,src_ip, dest_ip, protocol, src_port, dest_port, src_mac, dest_mac);
  
  if (PCAP_HELPER_DEBUG) {
    printf("values: %s\n", vp);
  }

  return vp;
}


