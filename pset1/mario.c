#include <cs50.h>
#include <stdio.h>

int main(void)
{
    printf("Height: ");
    int height = get_int();
    
    if(height < 1 || height > 23) {
        printf("Please enter a positive number no greater than 23\nRetry: ");
        height = get_int();
    }
    for(int i = 1; i <= height; i++){
        for(int j = 0; j < height - i; j++){
            printf(" ");
        }
        for(int j = 0; j < i; j++){
            printf("#");
        }
        printf("  ");
        for(int j = 0; j < i; j++){
            printf("#");
        }
        for(int j = 0; j < height - i; j++){
            printf(" ");
        }
        printf("\n");
    }
    
}