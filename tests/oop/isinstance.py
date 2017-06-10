class Animal:
    pass

class Dog(Animal):
    def __init__(self, name):
        self.name = name

    def bark(self):
        print "%s: bark" % self.name

class Cat(Animal):
    def __init__(self, name):
        self.name = name

    def meow(self):
        print "%s: meow" % self.name

puppy = Dog("Puppy")
tom = Cat("Tom")

print isinstance(puppy, Animal)
print isinstance(puppy, Dog)
print isinstance(puppy, Cat)

print isinstance(tom, Animal)
print isinstance(tom, Cat)
print isinstance(tom, Dog)

animals = [puppy, "String", tom]
for animal in animals:
    if isinstance(animal, Dog):
        animal.bark()
    if isinstance(animal, Cat):
        animal.meow()
