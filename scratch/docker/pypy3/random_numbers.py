import random
print("Starting list creation")
array = [random.randint(0,1000) for i in range(10000000)]
print("Starting sort")
array.sort()
print("Ending sort")
