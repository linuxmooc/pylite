def swap(vector, i, j):
    tmp = vector[i]
    vector[i] = vector[j]
    vector[j] = tmp

def qsort(vector, low, high):
    if low >= high:
        return
    pivot = vector[low]
    i = low + 1
    j = high
    while i != j:
        while i < j and vector[i] < pivot:
            i += 1
        while j > i and pivot < vector[j]:
            j -= 1
        if i != j:
            swap(vector, i, j)
    if vector[low] > vector[i]:
        swap(vector, low, i)
    qsort(vector, low, i - 1)
    qsort(vector, i, high)

def dump(vector):
    for i in vector:
        print i

def main():
    vector = [8, 16, 3, 14, 5, 12, 7, 10, 9, 1, 11, 6, 13, 4, 15, 2]
    low = 0
    high = len(vector) - 1
    qsort(vector, low, high)
    dump(vector)

main()
