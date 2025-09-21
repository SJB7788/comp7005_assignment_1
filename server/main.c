#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <sys/un.h>
#include <unistd.h>

int create_domain_socket(int domain, int stream_type, int protocol);
void bind_socket(struct sockaddr_un *addr, int sockfd, char path[]);

void handle_signal(int sig)
{
    printf("\nReceived signal %d, cleaning up...\n", sig);
    unlink("../socket");
    exit(EXIT_SUCCESS);
}

int main()
{
    struct sockaddr_un addr;
    int sockfd;
    char path[] = "../socket";

    sockfd = create_domain_socket(AF_UNIX, SOCK_STREAM, 0);
    bind_socket(&addr, sockfd, path);

    if (listen(sockfd, 5) == -1)
    {
        perror("Listen");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on %s\n", path);

    int exitflag = 1;
    char *response_message = "Received message";

    while (1)
    {
        int clientfd = accept(sockfd, NULL, NULL);
        if (clientfd == -1)
        {
            perror("Accept");
            exit(EXIT_FAILURE);
        }

        printf("Client connected\n");

        char buffer[128];
        ssize_t bytes = recv(clientfd, buffer, sizeof(buffer) - 1, 0);

        if (bytes > 0)
        {
            buffer[bytes] = '\0';
            printf("Received message: %s\n", buffer);
        }

        if (send(clientfd, response_message, strlen(response_message), 0) == -1)
        {
            perror("Send");
            exit(EXIT_FAILURE);
        }
    }

    unlink(path);
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

void bind_socket(struct sockaddr_un *addr, int sockfd, char path[])
{
    memset(addr, 0, sizeof(*addr));

    addr->sun_family = AF_UNIX;
    strcpy(addr->sun_path, path);

    if (bind(sockfd, (struct sockaddr *)addr, sizeof(*addr)) == -1)
    {
        perror("Bind");
        exit(EXIT_FAILURE);
    }

    return;
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