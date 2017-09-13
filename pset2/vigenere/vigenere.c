#include <stdio.h>
#include <cs50.h>
#include <ctype.h>
#include <string.h>

int main (int argc, string argv[])
{
    int j = 0;
    int x;
    
    if(argc == 2){
        string k = argv[1];
        //Test command line keyword, convert to lowercase, and reduce to index key numbers
        for(int i = 0, n = strlen(k); i < n; i++){
            if(isalpha(k[i])){
                k[i] = tolower(k[i]);
            } else {
                printf("Error: Command contained non-alphabetical character\n");
                return 2;
            }
        }
        
        //Obtain plaintext phrase and prefix the result
        printf("plaintext: ");
        string p = get_string();
        printf("ciphertext: ");
        
        //Ensure keyphrase is long enough by adding length if necessary
        while(strlen(k) < strlen(p)){
            strcat(k, k);
        }
        
        //Run through cipher process
        for(int i = 0, n = strlen(p); i < n; i++){
            x = k[j] - 97;
            if (p[i] >= 'a' && p[i] <= 'z'){
                if ((p[i]+x >= 'a' && p[i]+x <= 'z')){
                    printf("%c", (p[i] + x));
                }
                else {
                    printf("%c", (p[i] - (26 - x)));
                }
                j++;
            }
            else if (p[i] >= 'A' && p[i] <= 'Z'){
                if ((p[i]+x >= 'A' && p[i]+x <= 'Z')){
                    printf("%c", (p[i] + x));
                }
                else {
                    printf("%c", (p[i] - (26 - x)));
                }
                j++;
            }
            else {
                printf("%c", p[i]);
            }
        }
        
        printf("\n");
        return 1;
    }
    printf("Invalid Input");
    return 3;
}