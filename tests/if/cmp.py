class Person:
    def __init__(self, name, age):
        self.name = name
        self.age = age

    def __lt__(self, that):
        return self.age < that.age

    def __ge__(self, that):
        return self.age >= that.age

tom = Person('tom', 14)
jerry = Person('jerry', 12)

if tom < jerry:
    print 'tom is young'
else:
    print 'jerry is young'

if tom > jerry:
    print 'tom is old'
else:
    print 'jerry is old'
