def main():
    try:
        print 'try'
        return
    finally:
        print 'finally' 

print 'global start'
main()
print 'global end'
