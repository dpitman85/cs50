print("Height: ", end="")
height = int(input()) + 1

while height < 1 or height > 23:
    print("Please enter a positive number no greater than 23\nRetry: ", end="")
    height = int(input())

for i in range(height):
    for j in range(height-i):
        print(" ", end="")
    for j in range(i):
        print("#", end="")
    print("  ", end="")
    for j in range(i):
        print("#", end="")
    for j in range(height-i):
        print(" ", end="")
    print("");