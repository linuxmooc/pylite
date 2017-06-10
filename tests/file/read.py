import sys

file = open("file/read.py", "r")
while True:
    line = file.read(128)
    if not line:
        break
    sys.stdout.write(line)
