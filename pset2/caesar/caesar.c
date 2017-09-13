#include <stdio.h>
#include <cs50.h>
#include <ctype.h>
#include <string.h>

int main(int argc, string argv[])
{
    
    if(argc == 2){
        int k = atoi(argv[1]) % 26;
        printf("plaintext: ");
        string s = get_string();
        printf("ciphertext: ");
        for(int i = 0, n = strlen(s); i < n; i++){
            if (s[i] >= 'a' && s[i] <= 'z'){
                if ((s[i]+k >= 'a' && s[i]+k <= 'z')){
                    printf("%c", (s[i] + k));
                }
                else {
                    printf("%c", (s[i] - (26 - k)));
                }
            }
            else if (s[i] >= 'A' && s[i] <= 'Z'){
                if ((s[i]+k >= 'A' && s[i]+k <= 'Z')){
                    printf("%c", (s[i] + k));
                }
                else {
                    printf("%c", (s[i] - (26 - k)));
                }
            }
            else {
                printf("%c", s[i]);
            }
        }
    printf("\n");
    }
    else{
        printf("Invalid Inputs");
        return 2;
    }
    
    
    
}
