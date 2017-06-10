def f():
    for i in [11, 22, 33]:
        print i
f()

for i in [-2, -1, 0, 1, 2, 3, 4, 5]:
    if i < 0:
        continue
    print i
    if i >= 3:
        break
