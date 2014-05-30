import psycopg2
import sys

class database_manager:
    def __init__(self, config_path):
        self.init_conn(config_path)
        
    def init_conn(self, config_path):
        import ConfigParser
        config = ConfigParser.ConfigParser()
        config.read(config_path)
        
        dbprog = config.get("main", "dbprog")
        if dbprog == "postgresql":
            psql_creds = dict(config.items("postgresql"))

            keys_needed = ["database","username","password","host","port"]
            for key in keys_needed:
                if key not in psql_creds:
                    print "No", key, "in configuration file", config_path
                    sys.exit(1)

            try:
                self.conn = psycopg2.connect(database = psql_creds["database"], user = psql_creds["username"], password = psql_creds["password"], host = psql_creds["host"], port = psql_creds["port"])
            except Exception, e:
                print("%s \nCannot open a connection to database %s. \n Exiting.\n" % (e, psql_creds["database"]))
                sys.exit(1)
        
                
if '__main__' == __name__:
    dm = database_manager("../conf/db_creds.conf")
    
