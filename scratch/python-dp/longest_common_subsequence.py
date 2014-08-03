import numpy

def lcs_1(s1,s2):
    if len(s1) == 0 or len(s2) == 0:
        return 0
    elif s1[0] == s2[0]: 
        return lcs_1(s1[1:], s2[1:]) + 1
    else:
        return max(lcs_1(s1,s2[1:]), lcs_1(s1[1:],s2))

def lcs_2(s1,s2):
    n = len(s1)
    m = len(s2)
    mat = numpy.zeros(n,m)
    
s1 = "theryan"
s2 = "aryanirwin"

print lcs_1(s1, s2)
