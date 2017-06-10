for i in range(3):
    print i

def f():
    print i

f()

class Animal:
    count = 123 

    def __init__(self):
        print self.count
        print Animal.count

animal = Animal()
