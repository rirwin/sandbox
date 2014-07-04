import json
from pprint import pprint as pprint
import sys


if len(sys.argv) != 2:
    print "you need 1 argument to be path to the file (full or relative)"
    sys.exit(1)

file_path = sys.argv[1]
print "opening",file_path

try:
    f = open(file_path)
except Exception, e:
    print "File", file_path, "not loaded"
    print e
    sys.exit(1)

arr_of_dicts = []

def read_next_dictionary(f):
    depth = 0
    json_str = ""
    while True:
        c = f.read(1)
        if not c:
            break #EOF
        json_str += str(c)
        if c == '{':
            depth += 1
        elif c == '}':
            depth -= 1

        if depth == 0:
            break

    return json_str


while True:
    json_as_str = read_next_dictionary(f)
    try:
        json_dict = json.loads(json_as_str)
        arr_of_dicts.append(json_dict)
    except: 
        break # exception on loading json to end loop

 
print len(arr_of_dicts)
for jd in arr_of_dicts:
    if 'actor_attributes' in jd:
        jd_aa = jd['actor_attributes']
        if 'name' in jd_aa:
            print jd_aa['name']
        elif 'login' in jd_aa:
            print jd_aa['login']
    


