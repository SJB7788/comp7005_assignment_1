#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <sys/un.h>
#include <unistd.h>

int create_domain_socket(int domain, int stream_type, int protocol);
void bind_socket(struct sockaddr_un *addr, int sockfd, char path[]);

int main()
{
    struct sockaddr_un addr;
    int sockfd;
    char path[] = "/tmp/socket";

    sockfd = create_domain_socket(AF_UNIX, SOCK_STREAM, 0);
    bind_socket(&addr, sockfd, path);

    while (listen(sockfd, 5) != -1)
    {
        int clientfd = accept(sockfd, NULL, NULL);

        if (clientfd == -1)
        {
            perror("accept");
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
        perror("bind");
        exit(EXIT_FAILURE);
    }

    return;
}
