#python perf tester
import numpy as np
import math as math
import matplotlib.pyplot as plt
import os
from pylab import * # bad practice

#accum_size = [2, 5, 10, 20, 50, 100, 200, 500, 1000, 2000, 5000]
accum_size = [100, 1000]
filename = "~/Downloads/red2.5.cap"
dbname = "rirwin"
tablename = "test1"

for accum_i in accum_size:
    print accum_i
    #exec_str = "time ./main " + filename + " " + dbname + " " + tablename + " " + str(accum_i) + " > perf_test.res" 
    #print exec_str
    #os.system(exec_str);
    #a = os.system("cat perf_test.res")
    #print a

# hard coded results for first trial (no internet access to easily figure this out)
accum_sizes = np.array([1, 2, 5, 10, 20, 50, 100, 200, 500, 1000, 2000, 5000, 10000, 20000])
results = np.array([59.4, 32.0, 14.2, 8.62, 5.71, 3.85, 2.99, 2.68, 2.69, 2.55, 2.73, 3.34, 4.76, 7.05])

fig = plt.figure()
ax = fig.add_subplot(1,1,1)
plt.plot(accum_sizes, results)
ax.set_xscale('log')
plt.title('Block Transaction Size vs Completion Time for 130k Rows')
plt.xlabel('Rows entered at a time',fontsize=16)
plt.ylabel('Completion time (Seconds)',fontsize=16)
#plt.show()
plt.savefig('block_xaction_tradeoff.png',bbox_inches='tight')
