#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <sys/un.h>
#include <unistd.h>

int create_domain_socket(int domain, int stream_type, int protocol);
void connect_to_domain_socket(int sockfd, struct sockaddr_un *address, char *path);
void send_message(int sockfd, char *message);

int main() {
    struct sockaddr_un addr;

    int sockfd = create_domain_socket(AF_UNIX, SOCK_STREAM, 0);
    connect_to_domain_socket(sockfd, &addr, "/home/sjb/Documents/GitHub/comp7005_assignment_1/socket");
    send_message(sockfd, "I love honey");
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

void connect_to_domain_socket(int sockfd, struct sockaddr_un *address, char *path) {
    memset(address, 0, sizeof(*address));
    address->sun_family = AF_UNIX;
    strcpy(address->sun_path, path);
    printf("%s\n", address->sun_path);

    if (connect(sockfd, (struct sockaddr*) address, sizeof(*address)) == -1) {
        perror("Connect");
        exit(EXIT_FAILURE);
    }
}


void send_message(int sockfd, char *message) {
    if (send(sockfd, message, strlen(message), 0) == -1) {
        perror("Send");
        exit(EXIT_FAILURE);
    }
}

