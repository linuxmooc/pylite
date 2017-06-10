range_object = range(10)
iterator = range_object.__iter__()
while True:
    item = iterator.__next__()
    print(item)
