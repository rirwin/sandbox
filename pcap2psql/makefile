# sudo apt-get install libpcap-dev libpq-dev 
# https://help.ubuntu.com/community/PostgreSQL

LIBPQ_INCLUDE=/usr/include/postgresql
CC=gcc

all: gen_pcap_file read_pcap_file pcap2psql

pcap2psql: pcap2psql.c pcap_helper.h psql_writer.h
	$(CC) $< -o $@ -lpq -I$(LIBPQ_INCLUDE) -lpcap	

gen_pcap_file: gen_pcap_file.c
	$(CC) gen_pcap_file.c -o gen_pcap_file -lpcap

read_pcap_file: read_pcap_file.c
	$(CC) read_pcap_file.c -o read_pcap_file -lpcap

clean:
	rm pcap2psql
	rm gen_pcap_file
	rm read_pcap_file