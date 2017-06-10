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
    def __init__(self, method):
        self.method = method 

    def make_noise(self):
        print self.method()
        
def main():
    objects = []

    dog = Dog()
    objects.append(Adapter(dog.bark))

    cat = Cat()
    objects.append(Adapter(cat.meow))

    human = Human()
    objects.append(Adapter(human.speak))

    for object in objects:
        object.make_noise()

main()
