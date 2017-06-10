import sys

file = open("file/iter.py", "r")
for line in file:
    sys.stdout.write(line)
file.close()
