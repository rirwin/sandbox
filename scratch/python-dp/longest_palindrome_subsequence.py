import numpy
import pprint

def lps_1(s):
    if s is None:
        return 0
    elif len(s) == 1:
        return 1
    elif len(s) > 1 and s[0] != s[-1]:
        return max(lps_1(s[1:]), lps_1(s[:-1]))
    elif len(s) > 1 and s[0] == s[-1]:
        return 2 + lps_1(s[1:-1])


def lps_2(s):
    n = len(s)
    mat = numpy.eye(n)
    
    for gap in xrange(1,n):
        for i in xrange(n-gap):
            j = i + gap
            if s[i] == s[j] and gap == 1:
                mat[i][j] = 2
            elif s[i] == s[j]:
                mat[i][j] = mat[i+1][j-1] + 2
            else:
                mat[i][j] = max(mat[i][j-1], mat[i+1][j])

    pprint.pprint(mat)

    x = 0
    y = n-1
    sb = ""
    while x < y:

        if s[x] == s[y]:
            sb += s[x]
            x+=1
            y-=1
        elif mat[x][y-1] > mat[x+1][y]:
            y-=1
        else:
            x+=1

    sbc = str(sb)
    st = sbc[::-1]

    if x == y:
        sb += s[x] + st
    else:
        sb += st

    return mat[0][n-1], sb

s = "xyzabcdbad"
print lps_1(s)
print lps_2(s)
