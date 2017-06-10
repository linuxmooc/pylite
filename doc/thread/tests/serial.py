from threading import Thread

def add(low, high):
    sum = 0
    i = low 
    while (i < high):
        sum += 1
        i += 1
    return sum

n = 320000
print add(0, n)
