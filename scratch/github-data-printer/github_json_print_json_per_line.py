import json
from pprint import pprint as pprint
import sys


if len(sys.argv) != 3:
    print "you need 2 arguments"
    print "First to be path to the file (full or relative)"
    print "Second to be the path for the output file"
    sys.exit(1)

in_file_path = sys.argv[1]
out_file_path = sys.argv[2]

print "opening",in_file_path
print "writing to",out_file_path

try:
    fin = open(in_file_path, 'r')
    fout = open(out_file_path, 'w')
    
except Exception, e:
    print "File", in_file_path, "not loaded"
    print e
    sys.exit(1)

arr_of_dicts = []

def read_next_dictionary(fin):
    depth = 0
    json_str = ""
    while True:
        c = fin.read(1)
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
    json_as_str = read_next_dictionary(fin)
    if not json_as_str.startswith("{"):
        break
    fout.write(json_as_str + "\n")
    
fin.close()
fout.close()


