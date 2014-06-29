import random
import os
import numpy as np
import matplotlib.pyplot as plt
import networkx as nx
from find_house_values_dp import *
from find_house_values_optimal import *
import math
os.system("date")

# Install GNU Linear programming kit for
# checking of optimality. For ubuntu the  
# software is obtainable by:
#
# sudo apt-get install glpk python-numpy python-matplotlib
#
# Also, make sure you run `make` to compile stakeout.cpp file
#
# Check if system has glpsol command
has_glpk = os.system('which glpsol');

# Reforming output for true = 1
if has_glpk == 0: 
    has_glpk = 1;
else:
    has_glpk = 0;

max_num_houses = 10
num_runs_per_num_houses = 10;
non_optimal_output_flag = 0;

for num_houses in range(1,max_num_houses):
    for seed_i in range(num_runs_per_num_houses):
        random.seed(num_houses*num_runs_per_num_houses + seed_i)

        optimal_value = 0;

        house_values = [ 0 for i in range(num_houses) ] 
 
        ## Generate topology
        for house_i in range(num_houses):
            house_values[house_i] = random.randint(1,100)

        print "\nhouse values"
        print house_values

        dp_value = find_house_values_dp(house_values)
        print "Robbing algorithm ", dp_value

        ## Check with the optimal solution
        ## if system has glpk
        if has_glpk == 1:
            houses_to_rob, optimal_value = find_house_values_optimal(house_values)

            print 'Optimal robbing value ', optimal_value
            if num_houses > 1:
                print 'Optimal robbing: [',
                for house_i in range(num_houses-1):
                    if houses_to_rob[house_i] == 1:
                        print house_values[house_i],
                    else:
                        print 'x',
                    print ',',
            
                if houses_to_rob[num_houses-1] == 1:
                    print house_values[num_houses-1],
                else:
                    print 'x',
                print ']'


            if optimal_value != dp_value:
                non_optimal_output_flag = 1;
                print "Error mismatch of results (num_houses,seed_i) = (",num_houses,",", seed_i,")"

if has_glpk == 1:
    if non_optimal_output_flag == 1:
        print "\nError mismatch of results, see output to debug"
    else:
        print "\nDynamic programming results matched optimal results in every run\n"
