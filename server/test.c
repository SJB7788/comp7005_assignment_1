#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <sys/un.h>
#include <unistd.h>


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

void parse_arg(int argc, char* argv[], char** test, int* test_num) {
    if(sscanf(argv[1], "%s", *test) != 1) {
        printf("err: 1");
    };

    if (sscanf(argv[2], "%d", test_num) != 1) {
        printf("err 2");
    }

    printf("%s %d\n", *test, *test_num);
}

int main(int argc, char* argv[])
{
    int test;
    char* test_char;
    parse_arg(argc, argv, &test_char, &test);
}

// A - Z (65 - 90)
// a - z (97 - 122)