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

char* encrypt_ceaser_salad(char message[], int shift)
{
    for (int i = 0; message[i] != '\0'; i++)
    {
        char shifted = shift_char(message[i], shift);
        message[i] = shifted;
    }

    return message;
}

char* decrypt_ceaser_salad(char message[], int shift)
{
    for (int i = 0; message[i] != '\0'; i++)
    {
        char shifted = shift_char(message[i], -shift);
        message[i] = shifted;
    }

    return message;
}

int get_shift_from_message(char message[]) {
    char shift[16];

    for (int i = 0; message[i] != ' '; i++)
    {
        shift[i] = message[i];    
    }

    printf("%s\n", shift);

    return (int) shift;
    
}

int main()
{
    char test[] = "21 I Love Honey";
    int shift = 21;

    printf("%d\n", get_shift_from_message(test));

    char *res = encrypt_ceaser_salad(test, shift);
    printf("%s\n", res);
    char *decrypt = decrypt_ceaser_salad(res, shift);
    printf("%s\n", decrypt);
}

// A - Z (65 - 90)
// a - z (97 - 122)