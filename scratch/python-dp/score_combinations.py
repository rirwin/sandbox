import numpy

def combos(scores, total_score):
    c = numpy.zeros(shape=(k+1))
    c[0] = 1
    for score in scores:
        for i in xrange(score, total_score+1):
            c[i] += c[i-score] 

    return c

a = [3,7]
k = 24
c = combos(a,k)

