#include <cs50.h>
#include <math.h>
#include <stdio.h>

bool isValid(long long x);
long long powerOfTen(int exp);

int main(void)
{
    printf("Credit Card Number: ");
    long long ccn = get_long_long();
    int length = (int) floor(log10(ccn)) + 1;
    long long firstNum = (ccn - (ccn % powerOfTen(length-2))) / powerOfTen(length-2);
    
    if(isValid(ccn)){
        if((int) length == 15 && (firstNum == 34 || firstNum == 37)){
            printf("AMEX\n");
        }
        else if((int) length == 16 && (firstNum > 50 && firstNum < 56)){
            printf("MASTERCARD\n");
        }
        else if (((int) length == 16 || (int) length == 13) && firstNum - (firstNum % 10) == 40){
            printf("VISA\n");
        }
        else {
            printf("INVALID CARD\n");
        }
    }
    else {
        printf("INVALID CARD\n");
    }
    
}

bool isValid(long long x){
    int sum1 = 0;
    int sum2 = 0;
    long long y = x;
    
    while(y > 0){
        y -= y % 10;
        if(((y % 100) / 10) * 2 < 10){
            sum1 += ((y % 100) / 10) * 2;
        }
        else {
            sum1 += ((((y % 100) / 10) * 2) % 10) + 1;
        }
        y = (y - (y % 100)) / 100;
    }
    
    while(x > 0){
        sum2 += x % 10;
        x = (x - (x % 100)) / 100;
    }
    
    return (sum1 + sum2) % 10 == 0;
}

long long powerOfTen(int exp){
    long long p = 10;
    
    for(int i = exp; i > 1; i--){
        p *= 10;
    }
    
    return p;
}