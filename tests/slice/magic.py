class Array:
    def __init__(self):
        pass

    def __getitem__(self, slice):
        return "[%d, %d]" % (slice.start, slice.stop)

array = Array()
print array[1:3]
