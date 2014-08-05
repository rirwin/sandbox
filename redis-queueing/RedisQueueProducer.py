from RedisQueue import RedisQueue
import time

q = RedisQueue('test')

for i in xrange(100):
    q.put(i)
    print i, "put into queue"
    time.sleep(0.5)
