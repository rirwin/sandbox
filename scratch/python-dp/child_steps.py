import numpy

# child can hop 1, 2, or 3 steps.
# how many ways to go n steps

def steps1(n):
    if n == 1:
        return 1  # 1
    if n == 2:
        return 2  # 11, 2
    if n == 3:
        return 4  # 111, 12, 21, 3
    if n > 3:
        return steps(n-1) + steps(n-2) + steps(n-3)


cache = numpy.zeros(50)

def steps2(n):
    if n == 1:
        return 1  # 1
    if n == 2:
        return 2  # 11, 2
    if n == 3:
        return 4  # 111, 12, 21, 3
    if n > 3:
        if cache[n-1] != 0:
            sm1 = cache[n-1]
        else:
            sm1 = steps2(n-1)

        if cache[n-2] != 0:
            sm2 = cache[n-2]
        else:
            sm2 = steps2(n-2)
        
        if cache[n-3] != 0:
            sm3 = cache[n-3]
        else:
            sm3 = steps2(n-3)

        cache[n] = sm1 + sm2 + sm3
        return cache[n]


def steps3(n):

    c = numpy.zeros(50)
    c[1] = 1
    c[2] = 2
    c[3] = 4
    for i in xrange(4,n+1):
        c[i] = c[i-1] + c[i-2] + c[i-3]
        
    return c[n]

print steps3(30)
