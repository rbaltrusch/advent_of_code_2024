import re

# part 2: sum of products between do() and don't() instructions
with open("src/day_3/data.txt", "r", encoding="utf-8") as file:
    data = file.read()

processed_text = "".join(
    re.findall(r"(do\(\)|don't\(\)|mul\(\d+,\d+\))", f"do(){data}don't()")
)
matches = re.findall(r"do\(\)(.*?)don't\(\)", processed_text)
sum_ = sum(
    int(x) * int(y)
    for match in matches
    for x, y in re.findall(r"mul\((\d+),(\d+)\)", match)
)
print(sum_)
