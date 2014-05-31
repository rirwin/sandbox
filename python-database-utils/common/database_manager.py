import psycopg2
import sys
import ConfigParser
import json
import logging
import wrappers



class database_manager:

    def __init__(self, db_config_path):
        self.load_config(db_config_path)

    @wrappers.logger
    @wrappers.general_function_handler
    def load_config(self, db_config_path):
        config = ConfigParser.ConfigParser()
        config.read(db_config_path)
        logging.basicConfig(level = config.get("main", "loglevel") )
        
        # set up postgreSQL
        self.dbprog = config.get("main", "dbprog")
        if self.dbprog == "postgresql":
            self.conn = self.init_psql_conn(config)
            if self.conn is None:
                logging.error("\nCannot open a connection to database. Exiting.\n")
                sys.exit(1)

            self.schema_loader(config)


    @wrappers.logger
    @wrappers.general_function_handler
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

    @wrappers.logger
    @wrappers.general_function_handler
    def schema_loader(self, config):
        db_schema_path = config.get("main", "schemafile")
        self.schema_dict = json.load(open(db_schema_path))


    @wrappers.logger
    @wrappers.general_function_handler
    def reset_all_tables(self):
        for table_i in self.schema_dict:
            self.reset_table(table_i)

    @wrappers.logger
    @wrappers.general_function_handler
    def reset_table(self, table_str):
        self.drop_table(self.conn, table_str)
        schema_arr = self.schema_dict[table_str]
        schema_str = database_manager.translate_schema_array(self.dbprog, schema_arr)
        self.create_table(self.conn, table_str, schema_str)

    # called by <referent>.drop_table(<referent>.conn, table_str)
    @wrappers.logger
    @wrappers.database_function_handler
    def drop_table(self, cursor, table_str):
        cursor.execute("drop table if exists " + table_str) 

    @wrappers.logger
    @wrappers.database_function_handler
    def create_table(self, cursor, table_str, schema_str):
        cursor.execute("create table " + table_str + schema_str) 


    @wrappers.logger
    @wrappers.database_function_handler
    def simple_select_query(self, cursor, table_str, selected_columns_str, where_cond_str=""):
        if where_cond_str is not "":
            cursor.execute("select " + selected_columns_str + " from " + table_str + " where " + where_cond_str) 
        else:
            cursor.execute("select " + selected_columns_str + " from " + table_str) 
        #return cursor.fetchall()[0][0]
        return cursor.fetchall()


    @wrappers.logger
    @wrappers.database_function_handler
    def simple_insert_query(self, cursor, table_str, values_str):
        cursor.execute("insert into " + table_str + " values " + values_str) 
        

    @staticmethod
    @wrappers.logger
    @wrappers.general_function_handler
    def translate_schema_array(dbprog, schema_arr):
        if dbprog == "postgresql":
            schema_str = "("
            for col_i in schema_arr:
                schema_str += "\"" + col_i[0] + "\" " + col_i[1] + ","
        
        return schema_str[:-1] + ")"


    # called by database_manager.drop_table_static(conn, table_str)
    @staticmethod
    @wrappers.logger
    @wrappers.db_trx_exception_handler
    def drop_table_static(cursor, table_str):
        cursor.execute("drop table if exists " + table_str)

 
if '__main__' == __name__:

    # Full path (incl. file name) to database credentials
    db_config_path = "../conf/database.conf"

    dm = database_manager(db_config_path)

    dm.reset_all_tables()
    print dm.simple_select_query(dm.conn, "memory_report", "*")
    print dm.simple_insert_query(dm.conn, "memory_report", "('id1234', 1024124512, 8000)")
    print dm.simple_select_query(dm.conn, "memory_report", "*")    
    print dm.simple_insert_query(dm.conn, "memory_report", "('id5678', 1134124512, 4000)")
    print dm.simple_select_query(dm.conn, "memory_report", "*")

    print "Program continued to end"
