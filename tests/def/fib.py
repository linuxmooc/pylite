def fib(n):
    if n < 2:
        return 1
    return fib(n - 2) + fib(n - 1)

print fib(8)
