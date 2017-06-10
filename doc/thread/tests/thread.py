from threading import Thread

class Worker(Thread):
    def __init__(self, name):
        Thread.__init__(self)
        self.name = name

    def delay(self):
        for i in range(12345):
            pass

    def run(self):
        for i in range(8):
            print self.name
            self.delay()

workers = []
for i in range(3):
    worker = Worker(i)
    workers.append(worker)

for worker in workers:
    worker.start()

for worker in workers:
    worker.join()
