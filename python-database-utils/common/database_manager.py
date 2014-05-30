import psycopg2
import sys
import ConfigParser
import json
import logging


def dbwrap(func):
    """Wrap a function in an idomatic SQL transaction.  The wrapped function
    should take a cursor as its first argument; other arguments will be
    preserved.
    """

    def new_func(conn, *args, **kwargs):


        retval = None
        cursor = conn.cursor()
        try:
            cursor.execute("BEGIN")
            retval = func(cursor, *args, **kwargs)
            cursor.execute("COMMIT")
        except Exception, e:
            cursor.execute("ROLLBACK")
            logging.info(str(e) + " printed via logging.info")
            logging.warning(str(e) + " printed via logging.warning")
            #raise
        finally:
            cursor.close()

        return retval

    # Tidy up the help()-visible docstrings to be nice
    new_func.__name__ = func.__name__
    new_func.__doc__ = func.__doc__

    return new_func

@dbwrap
def do_something(cursor, val1=1, val2=2):
    #cursor.execute("select %s, %s", (val1, val2))
    cursor.execute("drop table foo")
    return cursor.fetchall()


class database_manager:

    def __init__(self, db_config_path, db_schema_path):

        self.load_config(db_config_path)
        self.schema_loader(db_schema_path)


    def load_config(self, db_config_path):
        config = ConfigParser.ConfigParser()
        config.read(db_config_path)
        
        dbprog = config.get("main", "dbprog")
        if dbprog == "postgresql":        
            self.init_psql_conn(config)

        logging.basicConfig(level = config.get("main", "loglevel") )

    # uncomment later and make this part of a package
    # @general_exception_handler_wrapper 
    def init_psql_conn(self, config):

        psql_creds = dict(config.items("postgresql"))
        try:
            self.conn = psycopg2.connect(database = psql_creds["database"], user = psql_creds["username"], password = psql_creds["password"], host = psql_creds["host"], port = psql_creds["port"])
        except Exception, e:
            print("%s \nCannot open a connection to database %s. \n Exiting.\n" % (e, psql_creds["database"]))
            sys.exit(1)
        

    def schema_loader(self, db_schema_path):
        try:
            self.schema_dict = json.load(open(db_schema_path))
        except Exception, e:
            print("%s \nCannot load schema file %s. \n Exiting.\n" % (e, db_schema_path))
            sys.exit(1)


    def reset_all_tables(self):
        for table_i in self.schema_dict:
            self.reset_table(table_i)

    def reset_table(self, table_str):
        database_manager.drop_table(self.conn, table_str)


    @staticmethod
    @dbwrap
    def drop_table(cursor, table_str):
        #cursor.execute("drop table if exists " + table_str)
        cursor.execute("drop table " + table_str)
        return cursor.fetchall()[0][0]



 
if '__main__' == __name__:

    # Full path (incl. file name) to database credentials
    db_conn_path = "../conf/database.conf"

    # Full path to database schema
    db_schema_path = "../conf/database_schema.json"

    dm = database_manager(db_conn_path, db_schema_path)

    print do_something(dm.conn)

    dm.reset_all_tables()

    print "Program continues"
