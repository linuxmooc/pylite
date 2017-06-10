def before_return():
    print 'before return'

def main():
    try:
        print 'try'
        return before_return()
    finally:
        print 'finally' 

print 'global start'
main()
print 'global end'
