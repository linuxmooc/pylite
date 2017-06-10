def loop(n):
    sum = 0
    list = []
    for i in range(n):
        list.append(i)
        sum += 1
    return sum

sum = 0
for i in range(1234):
    sum += loop(1234)
print sum
