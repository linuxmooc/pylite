def handle(stream, param_dict):
    number1 = int(param_dict["number1"])
    number2 = int(param_dict["number2"])
    result = number1 + number2
    response = "<h1>%d + %d = %d</h1>" % (number1, number2, result)
    response += "<a href=/index.html>Back to index</a>"
    stream.echo_200(response)
