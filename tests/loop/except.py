class Generator:
    def __init__(self, start, end):
        self.start = start
        self.end = end
        self.current = start

    def __iter__(self):
        return self

    def __next__(self):
        if self.current == (self.start + self.end) / 2:
            0 / 0
        current = self.current 
        self.current = self.current + 1
        return current

g = Generator(0, 10)
for i in g:
    print(i)
