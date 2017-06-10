from threading import Thread

class Computer(Thread):
    def __init__(self, low, high):
        Thread.__init__(self)
        self.low = low
        self.high = high
        self.result = 0

    def run(self):
        while True:
            pass

n = 20000
computer0 = Computer(0, n/2)
computer1 = Computer(n/2, n)
computer0.start()
computer1.start()
computer0.join()
computer1.join()
print computer0.result + computer1.result
