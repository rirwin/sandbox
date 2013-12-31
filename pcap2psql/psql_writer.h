#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libpq-fe.h>


static void
exit_nicely(PGconn *conn)
{
    PQfinish(conn);
    exit(1);
}


PGconn* init_conn(const char * conninfo){

  pcap_t *fp;
  char errbuf[PCAP_ERRBUF_SIZE];
  PGconn* conn;
  PGresult *res;

  /* Make a connection to the database */
  conn = PQconnectdb(conninfo);

  /* Check to see that the backend connection was successfully made */
  if (PQstatus(conn) != CONNECTION_OK) {

      fprintf(stderr, "Connection to database failed: %s",
	      PQerrorMessage(conn));
      exit_nicely(conn);// TODO return -1;
    }

  return conn;
}


void drop_table(PGconn* conn, const char * table) {

  char buff[256];
  PGresult *res;
  sprintf(buff,"DROP TABLE IF EXISTS %s", table);

  res = PQexec(conn, buff);
  if (PQresultStatus(res) != PGRES_COMMAND_OK) {
    fprintf(stderr, "Command failed: %s because %s", buff, PQerrorMessage(conn));
    PQclear(res);
    exit_nicely(conn);// TODO return -1;
  }
  
  PQclear(res);
}


void create_table(PGconn* conn, const char * table, const char * schema) {

  char buff[256];
  PGresult *res;
  sprintf(buff,"CREATE TABLE if not exists %s %s;",table, schema);

  res = PQexec(conn, buff);
  if (PQresultStatus(res) != PGRES_COMMAND_OK) {
    fprintf(stderr, "Command failed: %s because %s", buff, PQerrorMessage(conn));
    PQclear(res);
    exit_nicely(conn); // TODO return -1;
  }
  PQclear(res);
}


void insert_row(PGconn* conn, const char * table, const char * values){
  char buff[256];
  PGresult *res;

  sprintf(buff,"INSERT INTO %s values %s", table, values);
  res = PQexec(conn, buff);
  if (PQresultStatus(res) != PGRES_COMMAND_OK) {
    fprintf(stderr, "Command failed: %s because %s", buff, PQerrorMessage(conn));
    PQclear(res);
    exit_nicely(conn); // TODO return -1;
  }
  PQclear(res);
}

void insert_rows(PGconn* conn, const char * table, char ** row_vals, int num_rows){
  char *buff;
  PGresult *res;
  char tmp[256];
  int i;
  buff = malloc(num_rows*256);

  sprintf(buff,"INSERT INTO %s values %s,\n", table, row_vals[0]);
  for (i = 1; i < num_rows-1; i++) {
    sprintf(tmp,"%s,\n", row_vals[i]);
    strcat(buff,tmp);
  }
  sprintf(tmp,"%s;\n", row_vals[num_rows-1]);
  strcat(buff,tmp);

  res = PQexec(conn, buff);

  if (PQresultStatus(res) != PGRES_COMMAND_OK) {
    fprintf(stderr, "Command failed: %s because %s", buff, PQerrorMessage(conn));
    PQclear(res);
    exit_nicely(conn); // TODO return -1;
  }
  PQclear(res);
}
