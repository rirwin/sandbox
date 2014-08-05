
def add_parens(list_, lt_rem, rt_rem, st):
    
    if lt_rem < 0 or rt_rem < lt_rem:
        return

    if lt_rem == 0 and rt_rem == 0:
        list_.append(st)
        return
    
    if lt_rem > 0:
        new_st = str(st)
        new_st += '('
        add_parens(list_, lt_rem - 1, rt_rem, new_st)

    if rt_rem > 0:
        new_st = str(st)
        new_st += ')'
        add_parens(list_, lt_rem, rt_rem - 1, new_st)

    

list_ = []

add_parens(list_, 4, 4, "")

print list_
