def main():
    try:
        print 'try'
        0/0
    finally:
        print 'finally' 
        return  # don't rethrow the exception

print 'global start'
main()
print 'global end'
