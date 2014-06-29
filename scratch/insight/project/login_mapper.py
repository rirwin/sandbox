#!/usr/bin/env python
from operator import itemgetter
import sys
import json

# input comes from STDIN (standard input)
for line in sys.stdin:
    # remove leading and trailing whitespace
    line = line.strip()

    # emit login id
    try:
        json_dict = json.loads(line)
        print '%s\t%s' % (json_dict["actor_attributes"]["login"], 1)
    except:
        continue

    """

hadoop jar /opt/cloudera/parcels/CDH/lib/hadoop-0.20-mapreduce/contrib/streaming/hadoop-streaming.jar -file /home/ec2-user/ryan/sandbox/insight/project/login_mapper.py    -mapper /home/ec2-user/ryan/sandbox/insight/project/login_mapper.py -file /home/ec2-user/ryan/sandbox/insight/project/reducer.py   -reducer /home/ec2-user/ryan/sandbox/insight/project/reducer.py -input ryan/github/2012-04-11-15-lines.json -output ryan/github/out-8jun14-2

    # split the line into words
    words = line.split()
    # increase counters
    for word in words:
        # write the results to STDOUT (standard output);
        # what we output here will be the input for the
        # Reduce step, i.e. the input for reducer.py
        #
        # tab-delimited; the trivial word count is 1
        print '%s\t%s' % (word, 1)
    """
