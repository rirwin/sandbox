import numpy as np
from pylab import ylim
import matplotlib.pyplot as plt
import math
import sys
import time
import ConfigParser

opsmon_base = '/usr/local/ops-monitoring/'
config_path = opsmon_base + 'config/'

sys.path.append('/usr/local/ops-monitoring/common/')
import table_manager

class PsqlLinePlotter:

    def __init__(self, tbl_mgr):

        self.tbl_mgr = tbl_mgr
        self.curr_ts = 0

        # Read addresses to plot from config file                                                                     
        config = ConfigParser.ConfigParser()
        config.read(config_path + "/traffic_report.conf")
        self.addresses = []
        for k in config._sections["addresses"]:
            if k != "__name__": # ignore this key                                                                        
                self.addresses.append(config._sections["addresses"][k])
                
        self.xdata = []
        self.ydata = {}
        for addr_i in self.addresses:
            self.ydata[addr_i] = []

        self.hl, = plt.plot([], [])
        self.update_plot()

    def init_plot_colors(self):

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


    def update_plot(self):
        new_vars = self.get_var_all()
        curr_time = int(time.time())

        for addr_i in self.addresses:
            self.ydata[addr_i].append(new_vars[addr_i])
        self.xdata.append(curr_time)
        print self.ydata
        self.hl.set_xdata(np.append(self.hl.get_xdata(), curr_time))
        self.hl.set_ydata(np.append(self.hl.get_ydata(), 1))
        plt.draw()
        self.hl.set_xdata(np.append(self.hl.get_xdata(), curr_time+1))
        self.hl.set_ydata(np.append(self.hl.get_ydata(), 2))
        plt.draw()


            
    def get_var_all(self):
        new_vars = {}
        for addr_i in self.addresses:
            new_vars[addr_i] = self.exec_var_query(addr_i)
        return new_vars

    def exec_var_query(self, addr):
        cur = self.tbl_mgr.con.cursor()
        res = 0
        query_str = "select variance(v) from ops_traffic_report where id = '" + addr + "'"
        self.tbl_mgr.db_lock.acquire()
        try:
            cur.execute(query_str)
            res = cur.fetchall()
        except Exception, e:
            print "Exception:", e
            print "with query", query_str
            self.tbl_mgr.con.commit()
        cur.close()
        self.tbl_mgr.db_lock.release()

        return res


    def do_plot(self):
        ii = np.zeros(shape=(int(self.num_cols)))
        for i in range(self.num_cols):
            ii[i] = i#(i+0.0)/self.num_cols

        plt.figure()

        for vec_i in range(self.num_vectors):
            plt.plot(ii,self.vectors[vec_i],
                     color=self.my_colors[vec_i],
                     #label=self.legends_text[vec_i],
                     linestyle=self.my_linestyles[vec_i])
        plt.legend(loc='upper left')
        
        plt.yscale('log')
        #plt.ylim((0,10000))
        '''
        plt.suptitle('Multicast Cloud Connectivity vs Node Participation',fontsize=20)#\n 30 nodes, 0.4 communication range')
        plt.plot(ii,mean_res_conn_3s_4r,color='g',linestyle='--',label='Group of 3')
        plt.plot(ii,mean_res_conn_5s_4r,color='b',linestyle='-.',label='Group of 5')
        plt.plot(ii,mean_res_conn_7s_4r,color='r',linestyle=':',label='Group of 7')
        
        plt.legend(loc='upper left')
        plt.xlabel(u'Proportion of Nodes Participating (\u03b1)',fontsize=16)
        plt.ylabel('Mean Connectivity',fontsize=16)
        '''
        if self.save_figs == 0:
            plt.show()
        else:
            plt.savefig('plot.eps',bbox_inches='tight')


def main():

    db_type = "collector"
    debug = True
    tbl_mgr = table_manager.TableManager(db_type, config_path, debug)
    tbl_mgr.poll_config_store()

    
    plotter = PsqlLinePlotter(tbl_mgr)
    #lp = LinePlotter(np.matrix([range(5),range(1,6)]))
    #lp = LinePlotter([range(5)])

    

    save_figs = 0
    num_nodes = 30

    mean_res_conn_3s_4r = np.array([ 0.  ,  0.  ,  0.  ,  0.55,  1.35,  2.1 ,  2.7 ,  3.4 ,  3.65, 4.05,  4.4 ,  4.6 ,  4.8 ,  5.  ,  5.15,  5.2 ,  5.3 ,  5.4 , 5.5 ,  5.55,  5.55,  5.55,  5.55,  5.55,  5.55,  5.55,  5.55, 5.55,  5.55,  5.55])
    mean_res_conn_5s_4r = np.array([ 0.  ,  0.  ,  0.  ,  0.  ,  0.  ,  0.2 ,  0.9 ,  1.4 ,  2.05, 2.4 ,  2.8 ,  3.3 ,  3.4 ,  3.75,  3.9 ,  4.  ,  4.2 ,  4.3 , 4.4 ,  4.5 ,  4.5 ,  4.5 ,  4.55,  4.55,  4.55,  4.55,  4.55, 4.55,  4.55,  4.55])
    mean_res_conn_7s_4r = np.array([ 0.  ,  0.  ,  0.  ,  0.  ,  0.  ,  0.  ,  0.  ,  0.25,  0.9 , 1.45,  1.8 ,  2.25,  2.65,  2.8 ,  3.15,  3.35,  3.45,  3.75, 3.85,  3.9 ,  3.9 ,  3.95,  3.95,  3.95,  3.95,  3.95,  3.95, 3.95,  3.95,  3.95])

    ii = np.zeros(shape=(num_nodes))
    for i in range(num_nodes):
        ii[i] = (i+0.0)/num_nodes

    plt.figure()
    



if __name__ == '__main__':
    main()
