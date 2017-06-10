class Twin:
    def __init__(self, name):
        self.name = name
        self.other = None

def loop(n):
    sum = 0
    list = []
    for i in range(n):
        tom = Twin("tom")
        list.append(tom)

        jerry = Twin("jerry")
        list.append(jerry)

        tom.other = jerry
        jerry.other = tom
        sum += 1
    return sum

sum = 0
for i in range(512):
    sum += loop(1024)
print sum
