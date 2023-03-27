#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <cstdio>
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

    printf("ip: %s, port: %d\n", ip, port);

    struct sockaddr_in address;
    bzero(&address, sizeof address);
    address.sin_family = AF_INET;
    inet_pton(AF_INET, ip, (struct sockaddr*)&address.sin_addr);

    address.sin_port = htons(port);

    int sock = socket(PF_INET, SOCK_STREAM, 0);
    assert(sock >= 0);

    int ret = bind(sock,(struct sockaddr*)&address, sizeof address);
    printf("errno: %d, info: %s\n", errno, strerror(errno));
    assert(ret != -1);


    ret = listen(sock, 5);
    assert(ret != -1);

    struct sockaddr_in client;
    socklen_t client_addrlength;
    int connfd = accept(sock,(struct sockaddr*)&client, &client_addrlength);
    if (connfd < 0) {
        printf("errno is %d\n", errno);
    } else {
        char buffer[BUFSIZ];
        memset(buffer, '\0', sizeof buffer);
        ret = recv(connfd, buffer, BUFSIZ - 1, 0);
        printf("got %d bytes of normal data'%s'\n",ret,buffer);

        memset(buffer, '\0', sizeof buffer);
        ret=recv(connfd,buffer,BUFSIZ-1,MSG_OOB);
        printf("got %d bytes of oob data'%s'\n",ret,buffer);

        memset(buffer,'\0',BUFSIZ);
        ret=recv(connfd,buffer,BUFSIZ-1,0);
        printf("got %d bytes of normal data'%s'\n",ret,buffer);

        printf("I'l sleep 3s\n");
        sleep(3);
        struct sockaddr_in peer;
        socklen_t peerlen;
        char buf[BUFSIZ];
        ret = getpeername(connfd, (struct sockaddr*)&peer, &peerlen);
        if (ret == 0) {
            printf("remote ip: %s, remote port:%d\n",inet_ntop(AF_INET,&peer.sin_addr,buf, peerlen), ntohs(peer.sin_port));
        } else {
            printf("errno: %d, info: %s\n", errno, strerror(errno));
        }
        close(connfd);
    }
    close(sock);
    return 0;
}