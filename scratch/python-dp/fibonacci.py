import numpy

def fib1(i):
    if i == 0:
        return 0
    if i == 1:
        return 1
    return fib1(i - 1) + fib1(i - 2)


cache = numpy.zeros(30)


def fib2(i):
    if i == 0:
        return 0
    if i == 1:
        return 1
    if cache[i] != 0:
        return cache[i]

    cache[i] = fib1(i - 1) + fib1(i - 2)

    return cache[i]


def fib3(i):
    c = numpy.zeros(50)
    c[0] = 0
    c[1] = 1
    for idx in xrange(2,i+1):
        c[idx] = c[idx-1] + c[idx-2]
    return c[i]


print fib3(30)
