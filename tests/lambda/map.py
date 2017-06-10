def map(function, list):
    new_list = [] 
    for item in list:
        new_item = function(item)
        new_list.append(new_item)
    return new_list

print map(lambda x: x + x, [1, 2, 3])
