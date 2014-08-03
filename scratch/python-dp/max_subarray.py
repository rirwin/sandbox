
import numpy

a = [1,5,2,-5,3,-9,1,4,-2,4,5,-4]



def max_sub(a):
    cur_s = 0
    cur_a = [a[0]]
    max_s = 0
    for i in a:
        if cur_s + i > 0:
            cur_s = cur_s + i
            cur_a.append(i)
            if cur_s > max_s:
                max_s = cur_s
                max_a = list(cur_a)
        else:
            cur_s = 0
            cur_a = []

    return max_s, max_a

print max_sub(a)
        
