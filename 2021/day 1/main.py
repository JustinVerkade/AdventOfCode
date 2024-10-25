with open("data.txt") as file:
    data = file.readlines()
    buffer = [int(val) for val in data]
    increases = 0
    for i in range(len(buffer) - 1):
        if buffer[i] < buffer[i+1]:
            increases += 1
    print(increases)
