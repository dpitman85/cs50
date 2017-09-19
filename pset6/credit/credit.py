def isValid(x):
    sum1 = 0
    sum2 = 0
    y = x

    for i in range(len(y)-2,-1, -2):
        if (2*int(y[i])<10):
            sum1 += (int(y[i])*2)
        else:
            sum1 += ((int(y[i])*2)%10) + 1

    for i in range(len(y)-1,-1,-2):
        sum2 += int(y[i])

    return (sum1 + sum2) % 10 == 0;

print("Credit Card Number: ", end="")
ccn = input()
length = int(len(ccn))
firstNum = int(ccn[0:2])

if isValid(ccn):
    if length == 15 and (firstNum == 34 or firstNum == 37):
        print("AMEX")
    elif length == 16 and firstNum > 50 and firstNum < 56:
        print("MASTERCARD")
    elif (length == 16 or length == 13) and str(firstNum)[0] == 4:
        print("VISA")
    else:
        print("INVALID CARD")
else:
    print("INVALID CARD")

