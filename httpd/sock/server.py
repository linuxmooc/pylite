import socket
import os

HOST = "localhost"
PORT = 8080
server = socket.socket(socket.AF_INET, socket.SOCK_STREAM, 0)
server.bind((HOST, PORT))
server.listen(64)

while True:
    (client, address) = server.accept()
    data = client.recv(1024)
    print data
    client.close()
