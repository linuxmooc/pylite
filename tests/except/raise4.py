class MyException(Exception):
    pass

def main():
    raise MyException()

print "Start"
main()
print "End"
