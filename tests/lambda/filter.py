def filter(function, list):
    new_list = [] 
    for item in list:
        if function(item):
            new_list.append(item)
    return new_list

print filter(lambda x: x > 0, [-1, 1, -2, 2, -3, 3])
