def f(arg):
    def help(a, b):
        return a + b
    print help(arg, arg)

f(10)
