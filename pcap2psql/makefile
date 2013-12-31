#sudo apt-get install libpcap-dev libpq-dev 

LIBPQ_INCLUDE=/usr/include/postgresql
CC=gcc

all: main gen_pcap_file read_pcap_file

main: main.c psql_writer.h pcap_reader.h
	$(CC) $< -o $@ -lpq -I$(LIBPQ_INCLUDE) -lpcap

gen_pcap_file: gen_pcap_file.c
	$(CC) gen_pcap_file.c -o gen_pcap_file -lpcap

read_pcap_file: read_pcap_file.c
	$(CC) read_pcap_file.c -o read_pcap_file -lpcap

clean:
	rm main
	rm gen_pcap_file
	rm read_pcap_file