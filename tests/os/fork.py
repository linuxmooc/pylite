from os import fork

pid = fork()
if pid == 0:
    print 'Child'
else:
    print 'Parent'
