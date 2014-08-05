# Given m x n matrix of numbers find the max sum of 4 numbers straight

import numpy

class Result:
    def __init__(self, val, i, j, dir_tup):
        self.val = val
        self.i = i
        self.j = j
        self.dir_tup = dir_tup


def find_four(mat):

    num_rows = mat.shape[0]
    num_cols = mat.shape[1]
    
    global_max_res = Result(-1,-1,-1, () )
    directions = [(1,0), (0,1), (1,1), (-1,1)]

    for i in xrange(num_rows):
        for j in xrange(num_cols):
            ij_max_res = find_four_helper(i, j, mat, directions)
            if ij_max_res.val > global_max_res.val:
                global_max_res = ij_max_res

    return global_max_res


def find_four_helper(i, j, mat, directions):
    
    ij_max_res = Result(-1,-1,-1, () )

    num_rows = mat.shape[0]
    num_cols = mat.shape[1]

    for dir_i in directions:
        if valid_dir(i, j, num_rows, num_cols, dir_i):
            ij_res = calc_sum(i, j, mat, dir_i)
            if ij_res.val > ij_max_res.val:
                ij_max_res = ij_res

    return ij_max_res


def valid_dir(i, j, num_rows, num_cols, dir_tup):
    if i + dir_tup[0] * 3 < num_rows and j + dir_tup[1] * 3 < num_cols:
        return True
    else:
        return False


def calc_sum(i, j, mat, dir_tup):
    s = 0
    s += mat[i,j]
    s += mat[i + dir_tup[0], j + dir_tup[1]]
    s += mat[i + 2 * dir_tup[0], j + 2 * dir_tup[1]]
    s += mat[i + 3 * dir_tup[0], j + 3 * dir_tup[1]]
    return Result(s, i, j, dir_tup)
    

n = 30
m = 45
max_num = 100
rows = []
for row_idx in xrange(n):
    rows.append(numpy.random.random_integers(0, max_num, m))
    
mat = numpy.matrix(rows)


res = find_four(mat)

print res.val
print res.i, res.j
print res.dir_tup
