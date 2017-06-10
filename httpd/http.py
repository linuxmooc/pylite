import os
import sys
import config
import guess
import compute

class Stream(object):
    def __init__(self, socket):
        self.socket = socket
        self.reader = self.socket.makefile('rb')
        self.writer = self.socket.makefile('wb')

    def put_string(self, string):
        self.writer.write(string)

    def put_header(self, name, value):
        string = '%s: %s\r\n' % (name, value)
        self.writer.write(string)

    def new_line(self):
        self.writer.write('\r\n')

    def close(self):
        self.writer.flush()
        self.socket.close()

    def echo(self, status, body):
        if config.verbose:
            print "HTTP/1.0 %s" % status
        self.put_string("HTTP/1.0 %s\r\n" % status)
        self.put_header("Server", "Pyserver")
        self.put_header("Content-Type", "text/html")
        self.put_header("Content-Length", len(body))
        self.put_header("Connection", "close")
        self.new_line()
        self.put_string(body)

    def echo_200(self, string):
        self.echo("200 OK", string)

    def echo_404(self):
        self.echo("404 Not Found", "<b>File Not Found</b>")

    def echo_400(self):
        self.echo("400 Bad Request", "<h1>400 Bad Request</h1>")

    def echo_500(self):
        self.echo("500 Internal Error", "<h1>500 Internal Error</h1>")

    def echo_file(self, url):
        if url == "/":
            url = "/index.html" 
        path = "www" + url
        if not os.access(path, os.R_OK):
            self.echo_404()
            return

        array = []
        file = open(path, "r")
        while True:
            string = file.read(64)
            if not string:
                break
            array.append(string)
        file.close()
        content = "".join(array)
        self.echo_200(content)

    def get_request(self):
        line = self.reader.readline()
        if config.verbose:
            sys.stdout.write(line)
        return line

def handle_dynamic(stream, url):
    param_dict = {}
    index = url.find("?")
    if index >= 0:
        param_string = url[index + 1:]
        param_list = param_string.split("&")
        for param in param_list:
            (key, value) = param.split("=")
            param_dict[key] = value
    if config.verbose:
        print param_dict

    # url = /example.py?name=value
    #        ^      ^   
    #        1      index - 3
    # module_name = example
    module_name = url[1:index - 3]
    if config.verbose:
        print "__import__(%s)" % module_name
    module = __import__(module_name)
    module.handle(stream, param_dict)

def handle(socket):
    stream = Stream(socket)
    try:
        request = stream.get_request()
        fields = request.split(" ")

        if len(fields) < 3:
            stream.echo_400()
            return
        command = fields[0] 
        url = fields[1]
        protocol = fields[2]

        if command == "GET":
            if url.find(".py") != -1:
                handle_dynamic(stream, url)
            else:
                stream.echo_file(url)
        else:
            stream.echo_400()
    except Exception as error:
        print error
    finally:
        stream.close()
        sys.exit(0)
