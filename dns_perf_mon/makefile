LDNS=../extern/ldns-1.6.16
MYSQLPP=../extern/mysql++
CC=g++
# TODO
# include arguments properly
# link arguments
# pthread as well

# Be sure to run
# did sudo make install ... include paths needed....
# export LD_LIBRARY_PATH=/home/rirwin/Dropbox/thousandeyes/extern/ldns-1.6.16/.libs
# export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/rirwin/Dropbox/thousandeyes/extern/mysql++/lib/ # should point to /usr/local/lib ??
#./configure --with-mysql-lib=/usr/lib/i386-linux-gnu
#./configure --prefix="/home/rirwin/Dropbox/thousandeyes/extern/mysql++" --with-mysql-lib=/usr/lib/i386-linux-gnu

all: dns_perf_mon.o dns_perf_mon

dns_perf_mon: dns_perf_mon.o
	$(CC) $(LDNS)/lib/*o $(LDNS)/compat/*.o dns_perf_mon.o -o dns_perf_mon -pthread -lmysqlclient -lmysqlpp

dns_perf_mon.o: dns_perf_mon.cpp MySQLQueryHandler.hpp DNSQueryHandler.hpp
	$(CC) -c $< -o $@ -I$(MYSQLPP)/include/mysql++ -I/usr/include/mysql/

clean:
	-rm dns_perf_mon.o
	-rm dns_perf_mon
	-rm *~