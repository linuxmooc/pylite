def loop(n):
    sum = 0
    dict = {}
    for i in range(n):
        dict[i] = i
        sum += 1
    return sum

sum = 0
for i in range(54321):
    sum += loop(20)
print sum
