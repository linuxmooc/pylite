import socket
import os
import sys

HOST = "localhost"
PORT = 8080

def run_server():
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM, 0)
    server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    server.bind((HOST, PORT))
    server.listen(64)

    (client, address) = server.accept()
    data = client.recv(1024)
    data = data.upper()
    client.send(data)
    client.close()
    sys.exit(0)

def run_client():
    client = socket.socket(socket.AF_INET, socket.SOCK_STREAM, 0)
    client.connect((HOST, PORT))
    data = "hello world!"
    client.send(data)
    data = client.recv(32)
    print "client recv: %s" % data
    sys.exit(0)

def delay():
    for i in range(1234567):
        pass

pid = os.fork()
if pid == 0:
    delay()
    run_client()
else:
    run_server()
