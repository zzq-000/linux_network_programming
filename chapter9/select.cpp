#include <sys/select.h>
#include <cstdio>
#include <cstdlib>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cassert>
#include <cerrno>
#include <cstring>
#include <unistd.h>
#include <string>

#define BUFFER_SIZE 1024

int main(int argc, char* argv[]) {
    if (argc <= 2) {
        printf("usage: %s ip_address, port_number", basename(argv[0]));
    }

    const char* ip = argv[1];
    int port = atoi(argv[2]);

    struct sockaddr_in address;
    bzero(&address, sizeof address);
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    inet_pton(AF_INET, ip, &address.sin_addr);

    int sock = socket(PF_INET, SOCK_STREAM, 0);
    assert(sock >= 0);

    int ret = bind(sock, (struct sockaddr*)&address, sizeof address);
    assert(ret != -1);

    struct sockaddr_in client;
    socklen_t client_len = sizeof client;

    ret = listen(sock, 5);
    assert(ret != -1);

    int connfd = accept(sock, (struct sockaddr*)&client, &client_len);
    
    if (connfd < 0) {
        printf("errno: %d\n", errno);
    } else {
        char buf[BUFFER_SIZE];
        fd_set read_fds;
        fd_set exception_fds;
        FD_ZERO(&read_fds);
        FD_ZERO(&exception_fds);
        std::string hello = "hello, ";
        char send_data[BUFFER_SIZE];
        while (1) {
            memset(buf, '\0', sizeof buf);
            /*每次调用select前都要重新在read_fds和exception_fds中设置文件描述符
            connfd，因为事件发生之后，文件描述符集合将被内核修改*/
            FD_SET(connfd, &read_fds);
            FD_SET(connfd, &exception_fds);

            ret = select(connfd + 1, &read_fds, NULL, &exception_fds, NULL);

            if (ret < 0) {
                printf("selection failure\n");
                break;
            }

            /*对于可读事件，采用普通的recv函数读取数据*/
            if (FD_ISSET(connfd, &read_fds)) {
                ret=recv(connfd, buf, sizeof(buf) - 1, 0); 
                if(ret <= 0) {
                    printf("read_fd_isset\n");
                    break;
                }
                printf("get %d bytes of normal data: %s\n",ret,buf); 
                
                std::string send_data = hello + buf;
                ret = send(connfd, send_data.c_str(), send_data.size(), 0);

            } else if (FD_ISSET(connfd, &exception_fds)) {
                ret = recv(connfd, buf, sizeof(buf) - 1, MSG_OOB);
                if (ret <= 0) {
                    printf("exception_fd_isset\n");
                    break;
                }
                printf("get %d bytes of oob data: %s\n",ret,buf);
            }
        }
    }

    close(connfd);
    close(sock);
    return 0;
}