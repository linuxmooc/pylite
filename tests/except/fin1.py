def main():
    try:
        print 'main start'
        0/0
        print 'main end'
    finally:
        print 'finally' 

print 'global start'
main()
print 'global end'
