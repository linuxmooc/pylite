def is_safe(queens, n):
    i = 0
    while i < n:
        if queens[i] == queens[n] or n-i == queens[i]-queens[n] or n-i == queens[n]-queens[i]:
            return False 
        i += 1
    return True 

def place(queens, n):
    solutions = 0
    size = len(queens)
    if n == size:
        return 1
    pos = 0
    while pos < size:
        queens[n] = pos
        if is_safe(queens, n):
            solutions += place(queens, n + 1)
        pos += 1
    return solutions

def main():
    queens = [0, 0, 0, 0, 0, 0, 0, 0]
    solutions = place(queens, 0)
    print solutions

main()
