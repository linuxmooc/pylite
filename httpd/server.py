import socket
import os
import sys
import http
import config

def handle(client):
    if config.serial:
        http.handle(client)
    else:
        pid = os.fork()
        if pid == 0:
            http.handle(client)
            sys.exit(0)

def main(host, port):
    config.parse()
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM, 0)
    server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    server.bind((host, port))
    server.listen(64)

    while True:
        (client, address) = server.accept()
        handle(client)
        client.close()

main("localhost", 8080)
