import sys

j = 0

#Check to make sure there are the correct number of arguments
if len(sys.argv)!=2:
    print("Invalid Input")
    sys.exit(1)

#Test command line keyword, covert to lowercase
if sys.argv[1].isalpha():
    key = sys.argv[1].lower()
else:
    print("Invalid Input")
    sys.exit(2)

print("plaintext: ", end="")
p = input()
print("ciphertext: ", end="")

while len(key) < len(p):
    key += key

for i in p:
    a = key[j]
    x = ord(a) - 97
    if i >= "a" and i <= "z":
        if ord(i)+x >= ord("a") and ord(i)+x <= ord("z"):
            print(chr(ord(i) + x), end="")
        else:
            print(chr(ord(i)-(26-x)), end="")
        j += 1
    elif i >= "A" and i <= "Z":
        if ord(i)+x >= ord("A") and ord(i)+x <= ord("Z"):
            print(chr(ord(i) + x), end="")
        else:
            print(chr(ord(i)-(26-x)), end="")
        j += 1
    else:
        print(i, end="")

print("")
sys.exit(0)
