class Dog():
    def __init__(self):
        self.name = "Dog"

    def bark(self):
        return "woof"

class Cat():
    def __init__(self):
        self.name = "Cat"

    def meow(self):
        return "meow"

class Human():
    def __init__(self):
        self.name = "Human"

    def speak(self):
        return "hello"

class Adapter:
    def __init__(self, object, method):
        self.object = object
        self.method = method 

    def make_noise(self):
        print self.method(self.object)
        
def main():
    objects = []

    dog = Dog()
    objects.append(Adapter(dog, Dog.bark))

    cat = Cat()
    objects.append(Adapter(cat, Cat.meow))

    human = Human()
    objects.append(Adapter(human, Human.speak))

    for object in objects:
        object.make_noise()

main()
