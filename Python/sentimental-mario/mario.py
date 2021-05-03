from cs50 import get_int

height = 0
while (height < 1 or height > 8):
    height = get_int("Height: ")

for i in range(1, height + 1):
    print(" " * (height - i), end="")
    print("#" * i, end="  ")
    print("#" * i)