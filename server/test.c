#include <stdio.h>
#include <string.h>

char shift_char(char character, int shift)
{
    int mod = ((shift % 26) + 26) % 26; 

    if (character >= 'a' && character <= 'z')
    {
        return (char)('a' + ((character - 'a' + mod) % 26));
    }
    else if (character >= 'A' && character <= 'Z')
    {
        return (char)('A' + ((character - 'A' + mod) % 26));
    }

    return character; 
}

char *encrypt_ceaser_salad(char message[], int shift)
{
    for (int i = 0; message[i] != '\0'; i++)
    {
        char shifted = shift_char(message[i], shift);
        message[i] = shifted;
    }

    return message;
}

int main()
{
    char test[] = "abc";
    int shift = -2;

    char *res = encrypt_ceaser_salad(test, shift);
    printf("%s\n", res);
}

// A - Z (65 - 90)
// a - z (97 - 122)