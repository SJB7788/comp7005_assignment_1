#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <sys/un.h>
#include <unistd.h>

int create_domain_socket(int domain, int stream_type, int protocol);
void connect_to_domain_socket(int sockfd, struct sockaddr_un *address, char *path);
void send_message(int sockfd, char *message);
char *encrypt_ceaser_salad(char message[], int shift);
char *decrypt_ceaser_salad(char message[], int shift);
char shift_char(char character, int shift);

int main(int argc, char* argv[])
{
    // check for too many or too little cmd arguments
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <message> <shift> <socket_path>\n", argv[0]);
        return -1;
    }

    // place args in correct vars
    char *message = argv[1]; 
    int shift = atoi(argv[2]);
    char *path = argv[3];

    struct sockaddr_un addr;

    // create and connect to domain socket
    int sockfd = create_domain_socket(AF_UNIX, SOCK_STREAM, 0);
    connect_to_domain_socket(sockfd, &addr, path);

    // encrypt message 
    char *encrypted_message = encrypt_ceaser_salad(message, shift);
    // format message so it can also receive shift value
    char message_with_shift[128];
    snprintf(message_with_shift, sizeof(message_with_shift), "%s %d", message, shift);

    // send message and print message
    send_message(sockfd, message_with_shift);

    // receive message from server
    char recieved_message[128];
    ssize_t bytes = recv(sockfd, recieved_message, sizeof(recieved_message) - 1, 0);
    printf("Received message: %s\n", recieved_message);

    // decrypt the message
    char* decrypted_message = decrypt_ceaser_salad(recieved_message, shift);
    printf("Message decrypted: %s\n", decrypted_message);

    // close socket
    close(sockfd);
}

int create_domain_socket(int domain, int stream_type, int protocol)
{
    int sockfd = socket(domain, stream_type, protocol);

    if (sockfd == -1)
    {
        perror("Socket");
        exit(EXIT_FAILURE);
    }

    return sockfd;
}

void connect_to_domain_socket(int sockfd, struct sockaddr_un *address, char *path)
{
    memset(address, 0, sizeof(*address));
    address->sun_family = AF_UNIX;
    strcpy(address->sun_path, path);

    if (connect(sockfd, (struct sockaddr *)address, sizeof(*address)) == -1)
    {
        perror("Connect");
        exit(EXIT_FAILURE);
    }
}

void send_message(int sockfd, char *message)
{
    if (send(sockfd, message, strlen(message), 0) == -1)
    {
        perror("Send");
        exit(EXIT_FAILURE);
    }
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

char *decrypt_ceaser_salad(char message[], int shift)
{
    for (int i = 0; message[i] != '\0'; i++)
    {
        char shifted = shift_char(message[i], -shift);
        message[i] = shifted;
    }

    return message;
}

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
