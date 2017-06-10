def handle(stream, param_dict):
    number = int(param_dict["number"])
    answer = 5
    if number < answer:
        response = "Number %d is less than the answer" % number
    elif number > answer:
        response = "Number %d is greater than the answer" % number
    else:
        response = "Congratulations! Answer is %d" % answer
    response += "<br><a href=/index.html>Back to index</a>"
    stream.echo_200(response)
