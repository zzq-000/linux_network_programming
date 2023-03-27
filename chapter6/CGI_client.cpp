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
    if (argc <= 2) {
        printf("usage %s, ip_address port_number\n", basename(argv[0]));
    }
    const char* ip = argv[1];
    int port = atoi(argv[2]);


    struct sockaddr_in server_address;
    bzero(&server_address, sizeof server_address);
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    inet_pton(AF_INET, ip, &server_address.sin_addr);

    int sock = socket(PF_INET, SOCK_STREAM, 0);
    assert(sock >= 0);

    assert(sock >= 0);
    if (connect(sock, (struct sockaddr*)&server_address, sizeof server_address) == 0) {
        char buffer[BUFSIZ];
        memset(buffer, '\0', sizeof buffer);
        // send(sock, buffer, BUFSIZ, 0);
        while (recv(sock, buffer, BUFSIZ, 0) > 0) {
            printf("data: %s", buffer);
            memset(buffer, '\0', sizeof buffer);
        }

    } else {
        printf("errno: %d, info: %s\n", errno, strerror(errno));
    }
    close(sock);
    return 0;

}