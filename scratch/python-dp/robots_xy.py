
# question 9.2



# total of x + y moves
# x right moves, y down moves
#
# binomial expression 
# (x + y)
# (  x  )
#
# (x + y)! / (x! * y!)
#
def num_paths_math(x,y):
    import math
    fact = math.factorial
    return fact(x + y)/ (fact(x) * fact(y))


def num_paths_exp(i, j, x, y):
    if i + 1 == x and j == y:
        return 1
    if i == x and j + 1 == y:
        return 1
    nr = 0
    if i < x:
        nr = num_paths_exp(i + 1, j, x, y)
    nd = 0
    if j < y:
        nd = num_paths_exp(i, j + 1, x, y)
    return nr + nd


paths = []
def num_paths_save_paths(i, j, x, y, p):
    p.append((i,j))
    if i + 1 == x and j == y:
        p.append((i+1,j))
        paths.append(p)
        return 1
    if i == x and j + 1 == y:
        p.append((i,j+1))
        paths.append(p)
        return 1
    nr = 0
    if i < x:
        new_p = list(p)
        new_p.append((i+1,j))
        nr = num_paths_save_paths(i + 1, j, x, y, new_p)
    nd = 0
    if j < y:
        new_p = list(p)
        new_p.append((i,j+1))
        nd = num_paths_save_paths(i, j + 1, x, y, new_p)
    return nr + nd


print num_paths_math(6,7)
print num_paths_exp(0,0,6,7)
print num_paths_save_paths(0,0,6,7,[])
print len(paths)

print "Path 0: " + ') ->'.join(str(paths[0]).split('),'))
print "Path 100: " + ') ->'.join(str(paths[100]).split('),'))


