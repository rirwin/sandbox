from RedisQueue import RedisQueue
q = RedisQueue('test')

while True:
    item = q.get()
    print item, "received from queue"
    if item == "None":
        break
