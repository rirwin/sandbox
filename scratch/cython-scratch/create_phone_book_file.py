import random
import numpy

f = open('unsorted_phonebook.txt','w')

# phonebook
# add 0s for perf profiling
orig_np = numpy.array(range(100100,999999)) 
random.shuffle(orig_np)

for num in orig_np:
    f.write(str(num) + "\n")
