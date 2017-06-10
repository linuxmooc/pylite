class Stock:
    def __init__(self, name, price):
        self.name = name
        self.price = price
        self.observers = []

    def add_observer(self, observer):
        self.observers.append(observer)

    def update(self, delta):
        print "The stock price of %s has changed\n" % self.name
        self.price += delta

        for observer in self.observers:
            observer(self.price)
        print

class Kitten:
    def __init__(self, name):
        self.name = name

    def notify(self, price):
        print "To kitten %s, Current price is %d" % (self.name, price)

class Mice:
    def __init__(self, name):
        self.name = name

    def notify(self, price):
        print "To mice %s, Current price is %d" % (self.name, price)

def notify(price):
    print "To function, Current price is %d" % price

stock = Stock("GOOGLE", 100)
tom = Kitten("Tom")
jerry = Mice("Jerry")

stock.add_observer(notify)
stock.add_observer(tom.notify)
stock.add_observer(jerry.notify)
stock.update(10)
stock.update(-20)
