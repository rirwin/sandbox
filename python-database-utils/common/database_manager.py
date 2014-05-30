import psycopg2
import sys
import ConfigParser
import json
import logging
import wrappers


@wrappers.db_trx_exception_handler
def do_something(cursor, val1=1, val2=2):
    #cursor.execute("select %s, %s", (val1, val2))
    cursor.execute("drop table foo")
    return cursor.fetchall()


class database_manager:

    def __init__(self, db_config_path):
        self.load_config(db_config_path)

    def load_config(self, db_config_path):
        config = ConfigParser.ConfigParser()
        config.read(db_config_path)
        
        # set up postgreSQL
        dbprog = config.get("main", "dbprog")
        if dbprog == "postgresql":
            conn = self.init_psql_conn(config)
            if conn is None:
                logging.error("\nCannot open a connection to database. Exiting.\n")
                sys.exit(1)
            self.conn = conn

            #database_manager.init_psql_conn_static(config)
            self.schema_loader(config)

        logging.basicConfig(level = config.get("main", "loglevel") )


    def schema_loader(self, config):
        try:
            db_schema_path = config.get("main", "schemafile")
            self.schema_dict = json.load(open(db_schema_path))
        except Exception, e:
            print("%s \nCannot load schema file %s. \n Exiting.\n" % (e, db_schema_path))
            sys.exit(1)

    @wrappers.logger
    @wrappers.general_function_handler
    def init_psql_conn(self, config):

        conn = None
        psql_creds = dict(config.items("postgresql"))

        conn = psycopg2.connect(database = psql_creds["database"], user = psql_creds["username"], password = psql_creds["password"], host = psql_creds["host"], port = psql_creds["port"])

        return conn

    def schema_loader(self, config):
        try:
            db_schema_path = config.get("main", "schemafile")
            self.schema_dict = json.load(open(db_schema_path))
        except Exception, e:
            print("%s \nCannot load schema file %s. \n Exiting.\n" % (e, db_schema_path))
            sys.exit(1)


    def reset_all_tables(self):
        for table_i in self.schema_dict:
            self.reset_table(table_i)

    def reset_table(self, table_str):
        self.drop_table(self.conn, table_str)

    # called by <referent>.drop_table(<referent>.conn, table_str)
    @wrappers.logger
    @wrappers.database_function_handler
    def drop_table(self, cursor, table_str):
        #cursor.execute("drop table if exists " + table_str)
        cursor.execute("drop table " + table_str) # force exception
        return cursor.fetchall()[0][0]


    # called by database_manager.drop_table_static(conn, table_str)
    @staticmethod
    @wrappers.db_trx_exception_handler
    def drop_table_static(cursor, table_str):
        #cursor.execute("drop table if exists " + table_str)
        cursor.execute("drop table " + table_str) # force exception
        return cursor.fetchall()[0][0]

 
if '__main__' == __name__:

    # Full path (incl. file name) to database credentials
    db_config_path = "../conf/database.conf"

    dm = database_manager(db_config_path)

    #print do_something(dm.conn)

    dm.reset_all_tables()

    print "Program continues"
