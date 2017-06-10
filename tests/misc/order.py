def left():
    print 'left'
    return 0

def right():
    print 'right'
    return 0

print "left() + right()"
left() + right()
print

def call(a, b):
    pass
print "call(left(), right())"
call(left(), right())
print

def index():
    print 'index'
    return 0

def value():
    print 'value'
    return 0

def array():
    print 'array'
    return [0]

print "array()[index()]"
array()[index()]
print

print "array()[index()] = value()"
array()[index()] = value()
print
