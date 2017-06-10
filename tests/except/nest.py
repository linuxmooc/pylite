def main():
    try:
        try:
            0/0
        except:
            print 'Inner Exception'
    except:
        print 'Outter Exception' 

main()
