from threading import Thread
from threading import Condition

class Buffer:
    def __init__(self):
        self.count = 0
        self.item = None
        self.condition = Condition()

    def get(self):
        self.condition.acquire()
        while self.count == 0:
            self.condition.wait()

        item = self.item
        self.count -= 1

        self.condition.notify()
        self.condition.release()
        return item

    def put(self, item):
        self.condition.acquire()
        while self.count == 1:
            self.condition.wait()

        self.item = item
        self.count += 1

        self.condition.notify()
        self.condition.release()

class Producer(Thread):
    def __init__(self, buffer):
        Thread.__init__(self)
        self.buffer = buffer

    def run(self):
        for i in range(10):
            item = i + i
            #print "Produce %d" % item
            self.buffer.put(item)

class Consumer(Thread):
    def __init__(self, buffer):
        Thread.__init__(self)
        self.buffer = buffer

    def run(self):
        sum = 0
        for i in range(10):
            item = self.buffer.get()
            #print "    Consume %d" % item
            sum += item
        print sum

buffer = Buffer()
producer = Producer(buffer)
consumer = Consumer(buffer)

producer.start()
consumer.start()

producer.join()
consumer.join()
