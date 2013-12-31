#include <pcap.h>
#include <stdio.h>


const int LINE_LEN = 16;
int pkts_to_cap = 25; // TODO make arg
int pkts_capped = 0;


void print_packet(const struct pcap_pkthdr *header, const u_char *pkt_data);

main(int argc, char **argv) 
{
  pcap_if_t *all_devs;
  pcap_if_t *d;
  int inum;
  int pkt_cap_len = 65536;
  int read_timeout = 1000;
  int promiscuous_mode = 1; // 1 is true
  int i = 0;
  int res;
  pcap_t *adhandle;
  char errbuf[PCAP_ERRBUF_SIZE];
  pcap_dumper_t *dumpfile;
  struct pcap_pkthdr *header;
  const u_char *pkt_data;

  /* check command line */
  if (argc != 2) {
    printf("usage: %s filename\n", argv[0]);
    return -1;
  }

  /* retrieve the device list */
  if (pcap_findalldevs(&all_devs, errbuf) == -1) {
    fprintf(stderr, "Error in pcap_findalldevs: %s\n",errbuf);
    return -1;//exit(1);
  }
  
  for (d=all_devs; d; d = d->next) {
    printf("%d. %s", ++i, d->name);
    if (d->description)
      printf(" (%s)\n", d->description);
    else
      printf(" (No description available)\n");
  }

  if (i == 0) {
    fprintf(stderr, "No interfaces, make sure you run as sudo\n");
    return -1;
  }

  printf("Enter the interface number (1-%d):",i);
  scanf("%d", &inum);

  if (inum < 1 || inum > i) {
    printf("\nInterface number out of range\n");
    pcap_freealldevs(all_devs);
    return -1;
  }

  /* Jump to selected adapter */
  for (d = all_devs, i = 0; inum-1; d = d->next, i++);

  /* Open the adapter */
  if ( (adhandle = pcap_open_live(d->name,
				  pkt_cap_len,
				  promiscuous_mode,
				  read_timeout,
				  errbuf
				  ) ) == NULL) {

    fprintf(stderr,"\nUnable to open the adapter. %s\n",errbuf);
    pcap_freealldevs(all_devs);
    return -1;
  }
  
  /* Opening the dump file */
  dumpfile = pcap_dump_open(adhandle, argv[1]);
  if (dumpfile == NULL) {
    fprintf(stderr,"\nError opening the output file\n");
    return -1;
  }

  printf("\nlistening on %s...\n", d->description);

  /* free all_devs handle */
  pcap_freealldevs(all_devs);

  /* Start capture */
  while((res = pcap_next_ex( adhandle, &header, &pkt_data)) >= 0 &&
	pkts_capped < pkts_to_cap){

    print_packet(header, pkt_data);
    pcap_dump((u_char*)dumpfile, header, pkt_data);
    pkts_capped++;
  }

  pcap_dump_close(dumpfile);

  return 0;

}

void print_packet(const struct pcap_pkthdr *header, const u_char *pkt_data)
{
  int i;
  /* Print pkt timestamp and length */
  printf("%ld:%ld (%d)\n", header->ts.tv_sec, header->ts.tv_usec, header->len);

  /* print */
  for (i = 1; (i < header->caplen + 1) ; i++) {
    printf("%.2x ", pkt_data[i-1]);
    if ( (i % LINE_LEN) == 0) 
      printf("\n");
  }
  printf("\n\n");
}














