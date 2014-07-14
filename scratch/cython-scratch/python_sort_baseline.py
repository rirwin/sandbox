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
orig_np = numpy.array(range(10010,999999)) 
random.shuffle(orig_np)

# deep copy of orig
arr_np1 = numpy.array(orig_np) 

# deep copy of arr1
arr_np2 = numpy.array(orig_np) 

# deep copy to native list type
arr_nl1 = list(orig_np)

# deep copy to native list type
arr_nl2 = list(orig_np)

# last time checkpoint
chkpt=time.time()
diffs = []

arr_np1.sort()
diff, chkpt = calc_chkpt(chkpt)
diffs.append((round(diff,6),'np array sorted with a.sort()'))

numpy.sort(arr_np2)
diff, chkpt = calc_chkpt(chkpt)
diffs.append((round(diff,6),'np array sorted with numpy.sort(a)'))

arr_nl1.sort()
diff, chkpt = calc_chkpt(chkpt)
diffs.append((round(diff,6),'native list sorted with a.sort()'))

numpy.sort(arr_nl2)
diff, chkpt = calc_chkpt(chkpt)
diffs.append((round(diff,6),'native list sorted with numpy.sort(a)'))

# conclusion, use numpy.array over native list type when performance
# is critical, numpy.sort(native list) works okay, but I bet this is
# because numpy converts this to a numpy.array then numpy.sorts
pprint.pprint(diffs)


