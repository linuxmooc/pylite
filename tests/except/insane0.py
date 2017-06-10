def main():
    try:
        print 'try'
        0/0
    finally:
        print 'finally' 

print 'global start'
main()
print 'global end'
