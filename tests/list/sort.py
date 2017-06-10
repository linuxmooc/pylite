def print_vector(vector):
    size = 8
    i = 0
    while i < size:
        print vector[i]
        i = i + 1 

def select_sort(vector):
    size = 8
    i = 0
    while i < size:
        min = i
        j = i
        while j < size:
            if vector[j] < vector[min]:
                min = j
            j = j + 1
        tmp = vector[min]
        vector[min] = vector[i]
        vector[i] = tmp
        i = i + 1

vector = [4, 0, 5, 1, 6, 2, 7, 3]
select_sort(vector)
print_vector(vector)
