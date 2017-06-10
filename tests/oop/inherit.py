class Animal:
    def __init__(self, name, age):
        print "Animal.__init__()"
        self.name = name
        self.age = age

    def get_name(self):
        return self.name

    def get_age(self):
        return self.age

class Human(Animal):
    def __init__(self, name, age, occupation):
        Animal.__init__(self, name, age)
        self.occupation = occupation

    def dump(self):
        print self.get_name()
        print self.get_age()
        print self.occupation

human = Human("mike", 20, "worker")
human.dump()
