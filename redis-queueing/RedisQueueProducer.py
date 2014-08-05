from RedisQueue import RedisQueue
import time

q = RedisQueue('test')

for i in xrange(20):
    q.put(i)
    print i, "put into queue"
    time.sleep(0.5)

q.put("None")
