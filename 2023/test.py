print(sum([int(answer[0] + answer[-1]) for answer in ["".join([char for char in line if char.isdigit()]) for line in open("data.txt").readlines()]]))
