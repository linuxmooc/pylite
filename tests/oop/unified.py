class Vector(list):
    def __init__(self):
        list.__init__(self)
        self.count = 0

    def push_back(self, item):
        self.append(item)
        self.count += 1

vector = Vector()
vector.push_back(1)
vector.push_back(2)
vector.push_back(3)

print 'vector.count = %d' % vector.count
for item in vector:
    print item
