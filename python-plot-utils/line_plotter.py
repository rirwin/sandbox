import numpy as np
import matplotlib.pyplot as plt

import sys

class LinePlotter:

    def __init__(self, vectors, legend_text = [], titles_dict = {}, text_format_dict = {}, plot_format_array_of_dicts = []):

        (self.num_vectors, self.num_cols) = self.check_dims(vectors)
        self.vectors = vectors
        print "Data loaded:", self.num_vectors, "vectors of length",  self.num_cols

        self.legends_text = legend_text
        self.titles_dict = titles_dict
        self.text_format_dict = text_format_dict
        self.plot_format_array_of_dicts = plot_format_array_of_dicts
        self.plot = None
        self.save_figs =0
        self.my_colors = []#np.zeros(shape=(self.num_vectors))
        #self.init_plot_colors()
        self.do_plot()

    def check_dims(self,vectors):
        num_vectors = len(vectors[:])
        num_cols = len(vectors[0][:])

        row_leng = np.zeros(shape=(num_vectors))

        for i in xrange(1,num_vectors):
            if len(vectors[i]) != num_cols:
                print "Vectors must be of the same length"
                sys.exit(1)

        return (num_vectors, num_cols)

    def init_plot_colors(self):
        #np.zeros(shape=(self.num_vectors))
        self.colors.append('000000')#[0] = self.my_rgb(0.2)
        #self.my_colors[0] = '000000'#[0] = self.my_rgb(0.2)

    def my_rgb(self, rgb):
        return '000000'

    def do_plot(self):
        ii = np.zeros(shape=(self.num_cols))
        for i in range(self.num_cols):
            ii[i] = (i+0.0)/self.num_cols

        plt.figure()

        for vec_i in range(self.num_vectors):
            self.my_colors = []
            self.my_colors.append('000000')
            plt.plot(ii,self.vectors[vec_i],color=self.my_colors[0],label=vec_i)

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

    lp = LinePlotter([range(5),range(1,6)])
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
