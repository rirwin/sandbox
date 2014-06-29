import os
import re
def find_house_values_optimal(house_values):


    num_houses = house_values.__len__()
    
    os.system("rm opt_stakeout.sol");
    os.system("rm mathprog_stakeout.mod");

    file_mod = open('mathprog_stakeout.mod', 'w');
   
    ## Parameters
    file_mod.write('param NumHouses, integer, >= 1;\n');
    file_mod.write('set Houses, default {1..NumHouses};\n')

    # Houses to rob 
    file_mod.write('set RobbedHouses, within Houses;\n'); 
    file_mod.write('param house_value{i in Houses};\n');
    file_mod.write('\n');
    
    ## Variables


    # Robbed houses 
    file_mod.write('var h{i in RobbedHouses}, integer, >= 0, <= 1;\n');
    file_mod.write('/* h[i] = 0 if house i not robbed, 1 if robbed */\n');
    file_mod.write('\n');


    file_mod.write('s.t.\n');
    file_mod.write('\n');

    # Adjacent houses cannot be robbed
    # Python counts up from 0, mathprog counts up from 1.
    # Each constraint needs a unique name.
    for house_i in range(num_houses-1):
        file_mod.write('House_%d_%d: h[%d] + h[%d] <= 1;\n' % ((house_i+1),(house_i+2),(house_i+1),(house_i+2)))

    ## Objective Function
    file_mod.write('maximize obj: ');
    for house_i in range(num_houses-1):
        file_mod.write('h[%d]*house_value[%d] + ' % ((house_i+1), (house_i+1)));
    file_mod.write('h[%d]*house_value[%d];\n' % ((num_houses),(num_houses)));

    file_mod.write('/* objective is to maximize the total value of houses robbed on this street */\n');
    file_mod.write('\n');
    file_mod.write('solve;\n');
    file_mod.write('\n');
    file_mod.write('printf "Optimal - objective value %d\\n", ');
    for house_i in range(num_houses-1):
        file_mod.write('h[%d]*house_value[%d] + ' % ((house_i+1), (house_i+1)));
    file_mod.write('h[%d]*house_value[%d];\n' % ((num_houses),(num_houses)));
    file_mod.write('\n') 
    file_mod.write('printf "Houses Values:\\n";\n')
    file_mod.write('printf {i in RobbedHouses}: "%d\\n",house_value[i];\n')
    file_mod.write('printf "Houses Robbed:\\n";\n') # Could sum here
    file_mod.write('printf {i in RobbedHouses}: "%d\\n",h[i];\n')
    file_mod.write('\n');
   
        
    file_mod.write('data;\n');
    file_mod.write('\n');

    file_mod.write('param NumHouses := %d;\n' % num_houses);
    
    file_mod.write('param : RobbedHouses : house_value :=\n');
    for house_i in range(num_houses):
        file_mod.write('%d %d\n' % ((house_i+1),(house_values[house_i])));
    file_mod.write(';\n');
    file_mod.write('end;\n');
    file_mod.close();


    houses_to_rob = [ 0 for i in range(num_houses) ] 
    value = 0;

    # Put model and data file in model file
    a = os.system('glpsol --model "mathprog_stakeout.mod" > opt_stakeout.sol');
    if a == 0:
        #print 'Solver executed properly'
        f = open('opt_stakeout.sol')
        pattern1 = re.compile("Optimal - objective value")
        line = f.readline()
        while pattern1.match(line) == None:
            line = f.readline()
            m = pattern1.match(line)
            if m:
                value = int(line[25:line.__len__()])
                
        line = f.readline() # read 'Houses part of set of robbed houses:'
        line = f.readline()
        pattern2 = re.compile('House Values:')
        house_i = 0
        while (pattern2.match(line) == None and house_i < num_houses): 
            house_i_robbed =  int(line[0:line.__len__()-1])
            houses_to_rob[house_i] = house_i_robbed;
            line = f.readline()
            house_i = house_i + 1;
        line = f.readline()
        pattern3 = re.compile('Houses Robbed:')
        house_i = 0
        while (pattern3.match(line) == None and house_i < num_houses): 
            house_i_robbed =  int(line[0:line.__len__()-1])
            houses_to_rob[house_i] = house_i_robbed;
            line = f.readline()
            house_i = house_i + 1;
                                   
    else: # solver failed
        value = 0

    return houses_to_rob, value



