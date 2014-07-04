import json
from pprint import pprint as pprint
import sys


if len(sys.argv) != 2:
    print "you need 1 argument to be path to the file (full or relative)"
    sys.exit(1)

file_path = sys.argv[1]
print "opening",file_path

try:
    file_handle = open(file_path)
except Exception, e:
    print "File", file_path, "not loaded"
    print e
    sys.exit(1)

try:
    json_dict = json.load(file_handle)
except Exception, e:
    print "Unable to JSON"
    print e
    sys.exit(1)

pprint(json_dict)
