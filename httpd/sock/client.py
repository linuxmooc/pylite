import socket
import sys

HOST = "localhost"
PORT = 80
client = socket.socket(socket.AF_INET, socket.SOCK_STREAM, 0)
client.connect((HOST, PORT))
client.send("GET /index.html HTTP/1.0\r\n")
client.send("\r\n")

while True:
    data = client.recv(32)
    if not data:
        break
    sys.stdout.write(data)
