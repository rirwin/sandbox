# Why not use recursion for this? Okay don't answer that :)

def rev_str(s):

    if len(s) <= 1:
        return s

    return s[-1] + rev_str(s[1:-1]) + s[0]


print rev_str("a")
print rev_str("ab")
print rev_str("abcdef")
print rev_str("abcdefg")
