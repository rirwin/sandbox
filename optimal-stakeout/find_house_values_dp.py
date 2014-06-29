import os
import re
def find_house_values_dp(house_values):
    
    cmd = "./stakeout " 
    for i in range(len(house_values)):
        cmd = cmd + (str(house_values[i]))
        cmd = cmd + " "
    cmd = cmd + " > dp_stakeout.sol"
    print cmd

    a = os.system(cmd)
    if a == 0: # solver suceeded
        #print 'Solver executed properly'
        f = open('dp_stakeout.sol')
        
        pattern1 = re.compile("Expected Value:")
        line = f.readline()
        while pattern1.match(line) == None:
            line = f.readline()

        line = f.readline()
        value = int(line[0:line.__len__()])
                                   
    else: # solver failed
        value = 0

    return value



