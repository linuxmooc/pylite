def main():
    try:
        print 'main start'
        0/0
        print 'main end'
    except:
        print 'error' 

print 'global start'
main()
print 'global end'
