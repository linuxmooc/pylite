class MoveFileCommand():
    def __init__(self, src, dest):
        self.src = src
        self.dest = dest

    def execute(self):
        self.rename(self.src, self.dest)

    def undo(self):
        self.rename(self.dest, self.src)

    def rename(self, src, dest):
        print 'renaming %s to %s' % (src, dest)

commands = []
commands.append(MoveFileCommand('foo.txt', 'bar.txt'))
commands.append(MoveFileCommand('bar.txt', 'baz.txt'))

for command in commands:
    command.execute()

for command in commands:
    command.undo()
