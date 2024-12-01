import csv

with open("src/day_1/data.csv", "r", encoding="utf-8", newline="") as file:
    data = list(csv.reader(file, delimiter=";"))

# part 1: sum of differences of a pair of lists
row1, row2 = list(zip(*data))
total_difference = sum(abs(int(x) - int(y)) for x, y in zip(sorted(row1), sorted(row2)))
print(total_difference)

# part 2: weighted count of occurences of one list in another 
from collections import Counter

counter = Counter(map(int, row2))
weighted_sum = sum(x * counter.get(x, 0) for x in sorted(map(int, row1)))
print(weighted_sum)
