#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

int main(void)
{
    //Obtain user name
    printf("Please enter your name: ");
    string full_name = get_string();
    
    //Check each letter and add letters following space to initials string
    for(int i = 0, n = strlen(full_name); i < n; i++){
        if(toupper(full_name[i]) >= 'A' && toupper(full_name[i]) <= 'Z' && (full_name[i-1] == ' ' || i == 0)){
            printf("%c", toupper(full_name[i]));
        }
    }
    
    printf("\n");
}