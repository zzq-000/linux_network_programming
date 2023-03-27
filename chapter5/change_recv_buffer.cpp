#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cassert>
#include <cerrno>
#include <unistd.h>
int main(int argc, char* argv[]) {
    if (argc <= 3) {
        printf("usage: %s ip_address port_number buffer_size", basename(argv[0]));
    }

    const char* ip = argv[1];
    int port = atoi(argv[2]);
    int buffer_size = atoi(argv[3]);
    int len = sizeof buffer_size;

    struct sockaddr_in address;
    bzero(&address, sizeof address);
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    inet_pton(AF_INET, ip, &address.sin_addr);

    int sock = socket(PF_INET, SOCK_STREAM, 0);
    assert(sock >= 0);
    getsockopt(sock, SOL_SOCKET, SO_RCVBUF, &buffer_size, (socklen_t*)&len);
    // printf("gag\n");
    printf("current buffer size: %d\n", buffer_size);
    buffer_size = atoi(argv[3]);
    setsockopt(sock, SOL_SOCKET, SO_RCVBUF, &buffer_size, sizeof(buffer_size));
    getsockopt(sock, SOL_SOCKET, SO_RCVBUF, &buffer_size, (socklen_t*)&len);
    printf("current buffer size: %d\n", buffer_size);

    int ret = bind(sock, (struct sockaddr*)&address, sizeof address);
    assert(ret == 0);

    ret = listen(sock, 6);
    assert(ret == 0);

    struct sockaddr_in client;
    socklen_t client_len;
    int connfd = accept(sock, (struct sockaddr*)&client, &client_len);
    if (connfd < 0) {
        printf("errno: %d, errno: %s\n", errno, strerror(errno));
    } else {
        char buffer[BUFSIZ];
        memset(buffer, '\0', BUFSIZ);
        while (recv(connfd, buffer, BUFSIZ, 0)) {
            // printf("buffer: %s\n", buffer);

            printf("data size: %d\n", int(strlen(buffer)));
            memset(buffer, '\0', BUFSIZ);

            sleep(1);
        }
        close(connfd);
    }
    return 0;


}