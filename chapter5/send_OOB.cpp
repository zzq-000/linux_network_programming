#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cassert>
#include <unistd.h>
#include <cerrno>
int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("usage %s ip_address port_number", basename(argv[0]));
    }
    const char* ip = argv[1];
    int port = atoi(argv[2]);

    struct sockaddr_in server_address;
    // connect()
    bzero(&server_address, sizeof server_address);
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    inet_pton(AF_INET, ip, &server_address.sin_addr);

    int sock = socket(PF_INET, SOCK_STREAM, 0);
    assert(sock >= 0);

    if (connect(sock, (struct sockaddr*)&server_address, sizeof server_address) < 0) {
        printf("errno: %d, info: %s\n", errno, strerror(errno));
        printf("connection failed!\n");
    } else {
        const char* oob = "abc";
        const char* normal_data = "123";
        send(sock, normal_data, strlen(normal_data), 0);
        send(sock, oob, strlen(oob), MSG_OOB);
        send(sock, normal_data, strlen(normal_data), 0);
    }
    printf("I'l sleep 3s\n");
    sleep(3);
    struct sockaddr_in peer;
    socklen_t peerlen;
    char buf[BUFSIZ];
    int ret = getsockname(sock, (struct sockaddr*)&peer, &peerlen);
    if (ret == 0) {
        printf("remote ip: %s, remote port:%d\n",inet_ntop(AF_INET,&peer.sin_addr,buf, peerlen), ntohs(peer.sin_port));
    }
    close(sock);
    return 0;
}
