#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <cerrno>
#include <cstdlib>
#include <cassert>

int main(int argc, char* argv[]) {
    if (argc <= 2) {
        printf("usage %s ip_address port_number\n", basename(argv[0]));
        return 1;
    }
    const char* ip = argv[1];
    int port = atoi(argv[2]);

    //创建socket地址
    struct sockaddr_in address;
    bzero(&address, sizeof address);
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    inet_pton(AF_INET, ip, &address.sin_addr);

    int sock = socket(PF_INET, SOCK_STREAM, 0);
    assert(sock > 0);

    int ret = bind(sock,(struct sockaddr*)&address, sizeof address);
    assert(ret != -1);

    sleep(20);

    struct sockaddr_in client;
    socklen_t client_addrlength = sizeof client;
    int connfd = accept(sock,(struct sockaddr*)&client, &client_addrlength);

    if (connfd < 0) {
        printf("errno is %d\n", errno);
    }
    else {
        char remote[INET_ADDRSTRLEN];
        printf("connect ip: %s and port: %d\n", 
            inet_ntop(AF_INET, &client.sin_addr, remote, INET_ADDRSTRLEN),
            ntohs(client.sin_port));
        close(connfd);
    }
    close(sock);
}