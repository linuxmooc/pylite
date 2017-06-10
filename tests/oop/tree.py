class Tree:
    def __init__(self, left, data, right):
        self.left = left
        self.data = data
        self.right = right

    def visit_pre_order(self):
        print self.data
        if self.left:
            self.left.visit_pre_order()
        if self.right:
            self.right.visit_pre_order()

    def visit_in_order(self):
        if self.left:
            self.left.visit_pre_order()
        print self.data
        if self.right:
            self.right.visit_pre_order()

    def visit_post_order(self):
        if self.left:
            self.left.visit_pre_order()
        if self.right:
            self.right.visit_pre_order()
        print self.data

#  a
# b c
def main():
    b = Tree(None, "b", None)
    c = Tree(None, "c", None)
    a = Tree(b, "a", c)

    a.visit_pre_order()
    a.visit_in_order()
    a.visit_pre_order()

main()
