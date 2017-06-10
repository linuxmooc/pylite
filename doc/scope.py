class A:
    data = 123
    print 'inside class'

    def __init__(self):
        pass

    def test(self):
        #print data
        print A.data

print 'after class definition'
a = A()
a.test()
