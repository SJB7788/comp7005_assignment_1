#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <sys/un.h>
#include <unistd.h>

void handle_exit(void);
void manage_arguments(int argc, char *argv[], char **socket_path);
void error_message(char *message);
int valid_path(char *path);
int create_domain_socket(int domain, int stream_type, int protocol);
void bind_socket(struct sockaddr_un *addr, int sockfd, char path[]);
void start_listening(int sockfd, int capacity);
int accept_connection(int sockfd);
char *encrypt_ceaser_salad(char message[], int shift);
char *decrypt_ceaser_salad(char message[], int shift);
char shift_char(char character, int shift);
void send_message(int sockfd, char *message);

char *path;

int main(int argc, char *argv[])
{
    atexit(handle_exit);

    // validate and manage arguments
    manage_arguments(argc, argv, &path);

    struct sockaddr_un addr;
    int sockfd;

    // unlink socket in case of stale socket files
    unlink(path);
    // create domain socket and store fd
    sockfd = create_domain_socket(AF_UNIX, SOCK_STREAM, 0);
    // bind socket to the path given
    bind_socket(&addr, sockfd, path);

    // server listen
    start_listening(sockfd, 5);
    printf("Server listening on %s\n", path);

    // loop to keep server running forever
    while (1)
    {
        // get client fd
        int clientfd = accept_connection(sockfd);
        printf("Client connected\n");

        // create variable to receive message
        char received_message[128];

        // receive message and store it
        ssize_t bytes = recv(clientfd, received_message, sizeof(received_message) - 1, 0);

        // if message is valid
        if (bytes > 0)
        {
            // put an end string char at the end
            received_message[bytes] = '\0';

            // create a message and shift var
            char message[128];
            int shift;

            // find the last ' ' char which is where the shift value and message is divided
            char *last_char = strrchr(received_message, ' ');
            if (last_char) // if last char exists
            {
                // put an end character where the divider is at
                *last_char = '\0';
                // copy the message to the message var
                strcpy(message, received_message);
                // parse the string to get the shift int value
                shift = atoi(last_char + 1);
            }

            // create a buffer to copy the message to avoid changing the message variable for when we decrypt
            char copied_buffer[128];
            strcpy(copied_buffer, message);

            // decrypt the encrypted message
            char *decrypted_message = decrypt_ceaser_salad(copied_buffer, shift);
        }

        // send message back to client
        send_message(clientfd, received_message);
        // close client
        close(clientfd);
    }

    // in case while loop does not run, unlink socket
    unlink(path);
}

void handle_exit(void)
{
    unlink(path);
    exit(0);
}

void manage_arguments(int argc, char *argv[], char **socket_path)
{
    if (argc != 2)
    {
        error_message("not enough arguments.\n");
    }

    if (valid_path(argv[1]) != 1)
    {
        error_message("socket path must be valid.");
    }

    *socket_path = argv[1];
}

int valid_path(char *path)
{
    if (access(path, F_OK) == 0)
    {
        return 1;
    }

    return 0;
}

void error_message(char *message)
{
    fprintf(stderr, "%s\nUsage: <message> <shift> <socket_path>\n", message);
    exit(EXIT_FAILURE);
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

void start_listening(int sockfd, int capacity)
{
    if (listen(sockfd, 5) == -1)
    {
        perror("Listen");
        exit(EXIT_FAILURE);
    }
}

int accept_connection(int sockfd)
{
    int clientfd = accept(sockfd, NULL, NULL);
    if (clientfd == -1)
    {
        perror("Accept");
        exit(EXIT_FAILURE);
    }

    return clientfd;
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

void send_message(int sockfd, char *message)
{
    if (send(sockfd, message, strlen(message), 0) == -1)
    {
        perror("Send");
        exit(EXIT_FAILURE);
    }
}