#include <stdio.h>
#include <stdlib.h>
#include <libpq-fe.h>

static void
exit_nicely(PGconn *conn)
{
    PQfinish(conn);
    exit(1);
}

int main(int argc, char **argv) {
  const char *conninfo;
  PGconn *conn;
  PGresult   *res;
  int         nFields;
  int         i,j;

  /*
   * If the user supplies a parameter on the command line, use it as the
   * conninfo string; otherwise default to setting dbname=postgres and using
   * environment variables or defaults for all other connection parameters.
   */
  if (argc > 1)
    conninfo = argv[1];
  else
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
 
  res = PQexec(conn, "CREATE TABLE if not exists test1 (cap_time float8, pkt_len int4, cap_len int4, src_port int4, dest_port int4, protocol int4, src_ip cidr, dest_ip cidr, src_mac macaddr, dest_mac macaddr);");
  if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        fprintf(stderr, "CREATE TABLE if not exists test1 failed: %s", PQerrorMessage(conn));
        PQclear(res);
        exit_nicely(conn);
    }
    PQclear(res);

    


  return 0;
}
