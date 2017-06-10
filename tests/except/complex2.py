def main():
    try:
        print 'main start'
        0/0
        print 'main end'
    except:
        print 'except start'
        raise IOError()
        print 'except end'
    finally:
        print 'finally' 

print 'global start'
main()
print 'global end'
