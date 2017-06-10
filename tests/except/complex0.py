def main():
    try:
        try:
            print 'main start'
            0/0
            print 'main end'
        except:
            print 'except'
    finally:
        print 'finally' 

print 'global start'
main()
print 'global end'
