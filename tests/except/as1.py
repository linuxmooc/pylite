print "Start"
try:
    0/0
except Exception as error:
    print error
finally:
    print 'Finally'
print "End"
