class A:
    def __init__(self):
        print 'A.__init__'

class B(A):
    def __init__(self):
        print 'B.__init__'

b = B()
