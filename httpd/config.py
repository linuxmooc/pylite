import sys

verbose = False
serial = False

def parse():
    global verbose, serial
    for arg in sys.argv:
        if arg == "-serial":
            serial = True
        if arg == "-verbose":
            verbose = True
