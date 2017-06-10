def leibniz(n):
    pi = 0.0
    sign = 1.0
    i = 0
    while (i < n):
        pi += sign / (2 * i + 1)
        sign *= -1
        i += 1
    return pi * 4

print "pi = %f" % leibniz(54321)
