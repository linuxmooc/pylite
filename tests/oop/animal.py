class Animal:
    def __init__(self, name):
        self.name = name
        self.dump()

    def dump(self):
        print self.name

tom = Animal('tom')
tom.dump()
