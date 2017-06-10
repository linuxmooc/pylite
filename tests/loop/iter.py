class EvenGenerator:
        def __init__(self, start, end):
                self.start = start
                self.end = end
                self.current = start

        def __iter__(self):
                return self

        def __next__(self):
                if self.current > self.end:
                        raise StopIteration()
                current = self.current 
                self.current = self.current + 2
                return current

g = EvenGenerator(0, 10)
for i in g:
    print(i)
