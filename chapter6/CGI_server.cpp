#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cassert>
#include <cerrno>
#include <unistd.h>


int main(int argc, char* argv[]) {
    if (argc <= 2) {
        printf("usage %s ip_address port_number\n", basename(argv[0]));
    }

    const char* ip = argv[1];
    int port = atoi(argv[2]);

    struct sockaddr_in address;
    bzero(&address, sizeof address);
    address.sin_port = htons(port);
    address.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &address.sin_addr);

    int sock = socket(PF_INET, SOCK_STREAM, 0);
    assert(sock >= 0);

    int ret = bind(sock, (struct sockaddr*)&address, sizeof(address));
    assert(ret != -1);

    ret = listen(sock, 5);
    assert(ret!=-1);    

    struct sockaddr_in client;
    socklen_t len = sizeof client;
    int connfd = accept(sock, (struct sockaddr*)&client, &len);
    if (connfd < 0) {
        printf("errno: is: %d\n", errno);
    } else {
        close(STDOUT_FILENO);
        dup(connfd);
        printf("abcd\n");
        close(connfd);
    }
    close(sock);
    return 0;
}