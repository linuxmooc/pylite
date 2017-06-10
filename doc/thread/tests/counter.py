from threading import Thread
from threading import Lock

counter = 0
lock = Lock()

class Worker(Thread):
    def __init__(self, name):
        Thread.__init__(self)
        self.name = name

    def run(self):
        global counter
        for i in range(20000):
            lock.acquire()
            counter += 1
            lock.release()

workers = []
for i in range(3):
    worker = Worker(i)
    workers.append(worker)

for worker in workers:
    worker.start()

for worker in workers:
    worker.join()

print counter
