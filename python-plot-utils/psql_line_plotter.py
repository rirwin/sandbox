import numpy as np
from pylab import ylim
import matplotlib.pyplot as plt
import math
import sys
import time
import ConfigParser
from pprint import pprint as pprint

opsmon_base = '/usr/local/ops-monitoring/'
config_path = opsmon_base + 'config/'

sys.path.append('/usr/local/ops-monitoring/common/')
import table_manager

class PsqlLinePlotter:

    def __init__(self, tbl_mgr):

        self.tbl_mgr = tbl_mgr

        # Read addresses to plot from config file                                                                     
        config = ConfigParser.ConfigParser()
        config.read(config_path + "/traffic_report.conf")
        self.addresses = []
        for k in config._sections["addresses"]:
            if k != "__name__": # ignore this key                                                                        
                self.addresses.append(config._sections["addresses"][k])
        
        self.init_plot_styles()

        self.min_ts = int(self.exec_min_ts_query())
        self.curr_time = int(self.min_ts/1000000)+1

        self.xdata = []
        self.ydata = {}
        for addr_i in self.addresses:
            self.ydata[addr_i] = []

        for pts in xrange(20):
            self.iterate_data()

        self.do_plot()

    def init_plot_styles(self):

        self.my_colors = []
        self.my_colors.append('b')
        self.my_colors.append('g')
        self.my_colors.append('r')
        self.my_colors.append('c')
        self.my_colors.append('m')
        self.my_colors.append('y')
        self.my_colors.append('k')

        self.my_linestyles = []
        self.my_linestyles.append('-')
        self.my_linestyles.append('--')
        self.my_linestyles.append('-.')
        self.my_linestyles.append(':')
        self.my_linestyles.append('-.')
        self.my_linestyles.append('--')
        self.my_linestyles.append('-')


    def iterate_data(self):
        new_vars = self.get_var_all()
        new_vars = self.get_var_all()

        for addr_i in self.addresses:
            self.ydata[addr_i].append(new_vars[addr_i])
        self.xdata.append(self.curr_time)
        self.curr_time = self.curr_time + 1
  
            
    def get_var_all(self):
        new_vars = {}
        for addr_i in self.addresses:
            new_vars[addr_i] = self.exec_var_query(addr_i)
        return new_vars


    def exec_var_query(self, addr):
        cur = self.tbl_mgr.con.cursor()
        res = 0
        query_str = "select variance(v) from ops_traffic_report where id = '" + addr + "' and ts < " + str(self.curr_time * 1000000)
        self.tbl_mgr.db_lock.acquire()
        try:
            cur.execute(query_str)
            if res is None:
                res = 0
            else:
                res = cur.fetchall()[0]
        except Exception, e:
            print "Exception:", e
            print "with query", query_str
            self.tbl_mgr.con.commit()
        cur.close()
        self.tbl_mgr.db_lock.release()

        return res


    def exec_raw_query(self, addr):
        cur = self.tbl_mgr.con.cursor()
        res = 0
        query_str = "select distinct(v) from ops_traffic_report where id = '" + addr + "' and ts < " + str(self.curr_time * 1000000) + " and ts > " + str(self.curr_time-1 * 1000000)
        self.tbl_mgr.db_lock.acquire()
        try:
            cur.execute(query_str)
            if res is None:
                res = 0
            else:
                res = cur.fetchall()[0]
        except Exception, e:
            print "Exception:", e
            print "with query", query_str
            self.tbl_mgr.con.commit()
        cur.close()
        self.tbl_mgr.db_lock.release()

        return res


    def exec_min_ts_query(self):
        cur = self.tbl_mgr.con.cursor()
        res = 0
        query_str = "select min(ts) from ops_traffic_report"
        self.tbl_mgr.db_lock.acquire()
        try:
            cur.execute(query_str)
            res = cur.fetchall()[0][0]
        except Exception, e:
            print "Exception:", e
            print "with query", query_str
            self.tbl_mgr.con.commit()
        cur.close()
        self.tbl_mgr.db_lock.release()

        return res


    def do_plot(self):

        plt.figure()

        # start counting at 1 instead of seconds since epoch
        first_t = self.xdata[0]
        xdata_sec = [(t-first_t) for t in self.xdata]

        # iterate through styles
        style_i = 0

        for addr_i in self.addresses:
            plt.plot(xdata_sec, self.ydata[addr_i],
                     color=self.my_colors[style_i],
                     label=addr_i,
                     linestyle='--')
            style_i += 1

        plt.legend(loc='lower right')
        plt.title('Destination Traffic Variance Characteristic')
        plt.xlabel('Variance of packets to destination per sec',fontsize=16)
        plt.ylabel('Time in Seconds',fontsize=16)

        plt.show()



def main():

    db_type = "collector"
    debug = True
    tbl_mgr = table_manager.TableManager(db_type, config_path, debug)
    tbl_mgr.poll_config_store()

    
    plotter = PsqlLinePlotter(tbl_mgr)
    



if __name__ == '__main__':
    main()
