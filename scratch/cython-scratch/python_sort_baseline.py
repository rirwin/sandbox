import random
import numpy
import time
import pprint

def calc_chkpt(chkpt):
    curr_time = time.time()
    diff = curr_time - chkpt
    chkpt = curr_time
    return diff, chkpt

# phonebook
# add 0s for perf profiling
arr_np1 = numpy.array(range(10010,999999)) 
random.shuffle(arr_np1)

# deep copy of arr1
arr_np2 = numpy.array(arr_np1) 

# deep copy to native list type
arr_na1 = list(arr_np1)

# deep copy to native list type
arr_na2 = list(arr_np1)

# last time checkpoint
chkpt=time.time()
diffs = []

arr_np1.sort()
diff, chkpt = calc_chkpt(chkpt)
diffs.append((round(diff,6),'np array sorted with a.sort()'))

numpy.sort(arr_np2)
diff, chkpt = calc_chkpt(chkpt)
diffs.append((round(diff,6),'np array sorted with numpy.sort(a)'))

arr_na1.sort()
diff, chkpt = calc_chkpt(chkpt)
diffs.append((round(diff,6),'native array sorted with a.sort()'))

numpy.sort(arr_na2)
diff, chkpt = calc_chkpt(chkpt)
diffs.append((round(diff,6),'native array sorted with numpy.sort(a)'))

# conclusion, use numpy.array over native list type when performance
# is critical
pprint.pprint(diffs)


