class N:
    def __add__(self, that):
        print "__add__"

    def __sub__(self, that):
        print "__sub__"

    def __mul__(self, that):
        print "__mul__"

    def __div__(self, that):
        print "__div__"

    def __mod__(self, that):
        print "__mod__"

    def __lt__(self, that):
        print "__lt__"

    def __le__(self, that):
        print "__le__"

    def __eq__(self, that):
        print "__eq__"

    def __ne__(self, that):
        print "__ne__"

    def __gt__(self, that):
        print "__gt__"

    def __ge__(self, that):
        print "__ge__"

    def __len__(self):
        print "__len__"
        return 0

n1 = N()
n2 = N()

n1 + n2
n1 - n2
n1 * n2
n1 / n2
n1 % n2

n1 < n2
n1 <= n2
n1 == n2
n1 != n2
n1 >= n2
n1 > n2
