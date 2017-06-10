class Person:
    def __init__(self, name, gender):
        self.name = name 
        self.gender = gender

    def __str__(self):
        return self.name + "/" + self.gender

tom = Person("tom", "male")
jerry = Person("jerry", "female")
print tom
print jerry
