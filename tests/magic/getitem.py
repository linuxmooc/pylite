class Map:
    def __init__(self):
        pass

    def __getitem__(self, key):
        return key.upper()

    def __setitem__(self, key, value):
        print key
        print value

map = Map()
print map["abc"]
map["xxx"] = "yyy"
