def loop(n):
    sum = 0
    string = ""
    for i in range(n):
        string += "x"
        sum += 1
    return sum

sum = 0
for i in range(12345):
    sum += loop(100)
print sum
