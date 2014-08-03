


def comp_dist(s1, s2):
    
    if len(s1) == 0:
        return len(s2)

    if len(s2) == 0:
        return len(s1)

    if s1[-1] == s2[-1]:
        return comp_dist(s1[:-1], s2[:-1]) 

    return min(comp_dist(s1, s2[:-1]), comp_dist(s1[:-1], s2), comp_dist(s1[:-1], s2[:-1]))  + 1


cache = {}

def comp_dist_dp(s1, s2):
    
    if len(s1) == 0:
        cache[(s1,s2)] = len(s2)
        return len(s2)

    if len(s2) == 0:
        cache[(s1,s2)] = len(s1)
        return len(s1)

    if s1[-1] == s2[-1]:
        if (s1,s2) in cache:
            return cache[(s1,s2)]
        return comp_dist_dp(s1[:-1], s2[:-1]) 

    if (s1, s2[:-1]) in cache:
        c1 = cache[(s1,s2[:-1])]
    else:
        c1 = comp_dist_dp(s1, s2[:-1])
        cache[(s1,s2[:-1])] = c1

    if (s1[:-1], s2) in cache:
        c2 = cache[(s1[:-1], s2)]
    else:
        c2 = comp_dist_dp(s1[:-1], s2)
        cache[(s1[:-1],s2)] = c2

    if (s1[:-1], s2[:-1]) in cache:
        c3 = cache[(s1[:-1], s2[:-1])]
    else:
        c3 = comp_dist_dp(s1[:-1], s2[:-1])
        cache[(s1[:-1],s2[:-1])] = c3

    return min(c1, c2, c3) + 1


s1 = 'saturday is not funny'
s2 = 'sunday is funny'
    
print comp_dist_dp(s1, s2)
print cache
