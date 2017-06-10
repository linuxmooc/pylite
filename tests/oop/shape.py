class Square:
    def __init__(self, width):
        self.width = width

    def area(self):
        return self.width * self.width

    def move(self):
        print "Square move"

class Rectangle:
    def __init__(self, width, height):
        self.width = width
        self.height = height

    def area(self):
        return self.width * self.height

    def move(self):
        print "Rectangle move"

def move_shape(shapes):
    shapes[0].move()
    shapes[1].move()

def show_area(shapes):
    print shapes[0].area()
    print shapes[1].area()

square = Square(10)
rectangle = Rectangle(10, 20)
shapes = [square, rectangle]
move_shape(shapes)
show_area(shapes)
