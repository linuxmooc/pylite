def reduce(function, list, initial_value):
    result = initial_value
    for item in list:
        result = function(result, item)
    return result 

print reduce(lambda x, y: x + y, range(100), 0)
