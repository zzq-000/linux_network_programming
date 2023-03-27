#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cassert>
#include <cerrno>
#include <unistd.h>

int main(int argc, char* argv[]) {
    if (argc <= 3) {
        printf("usage %s, ip_address port_number buffer_size\n", basename(argv[0]));
    }
    const char* ip = argv[1];
    int port = atoi(argv[2]);
    int buffer_size = atoi(argv[3]);
    int len = sizeof(buffer_size);

    struct sockaddr_in server_address;
    bzero(&server_address, sizeof server_address);
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    inet_pton(AF_INET, ip, &server_address.sin_addr);

    int sock = socket(PF_INET, SOCK_STREAM, 0);
    getsockopt(sock, SOL_SOCKET, SO_SNDBUF, &buffer_size, (socklen_t*)&len);
    printf("current buffer size: %d\n", buffer_size);
    buffer_size = atoi(argv[3]);
    setsockopt(sock, SOL_SOCKET, SO_SNDBUF, &buffer_size, sizeof(buffer_size));
    getsockopt(sock, SOL_SOCKET, SO_SNDBUF, &buffer_size, (socklen_t*)&len);
    printf("current buffer size: %d\n", buffer_size);

    assert(sock >= 0);
    if (connect(sock, (struct sockaddr*)&server_address, sizeof server_address) == 0) {
        char buffer[BUFSIZ];
        memset(buffer, 'a', sizeof buffer);
        send(sock, buffer, BUFSIZ, 0);

    } else {
        printf("errno: %d, info: %s\n", errno, strerror(errno));
    }
    close(sock);
    return 0;

}