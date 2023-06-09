#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <cstring>
#include <cassert>
#include <sys/poll.h>
#include <poll.h>
#include <fcntl.h>

#define BUFFER_SIZE 64

int main(int argc, char* argv[]) {
    if (argc <= 2) {
        printf("usage %s ip_address, port_number\n", basename(argv[0]));
    }

    const char* ip = argv[1];
    int port = atoi(argv[2]);

    struct sockaddr_in address;
    bzero(&address, sizeof address);
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    inet_pton(AF_INET, ip, &address.sin_addr);

    int sockfd = socket(PF_INET, SOCK_STREAM, 0);
    assert(sockfd);

    if (connect(sockfd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        printf("connection failed!\n");
        close(sockfd);
        return 0;
    }
    pollfd fds[2];

    fds[0].fd = 0;
    fds[0].events = POLLIN;
    fds[0].revents = 0;

    fds[1].fd = sockfd;
    fds[1].fd = POLLIN | POLLRDHUP;
    fds[1].fd = 0;

    char read_buf[BUFFER_SIZE];
    int pipefd[2];
    int ret = pipe(pipefd);
    assert(ret != -1);

    while (1) {
        ret = poll(fds, 2, -1);
        if(ret<0) {
            printf("poll failure\n");
            break;
        }
        if (fds[1].revents & POLLRDHUP) {
            printf("server close the connection\n");
            break;
        } else if(fds[1].revents & POLLIN) {
            memset(read_buf, '\0', sizeof read_buf);
            ret = recv(fds[1].fd, read_buf, BUFFER_SIZE-1, 0);
            printf("%s\n", read_buf);
        }
        if(fds[0].revents&POLLIN) {
            /*使用splice将用户输入的数据直接写到sockfd上(零拷贝)*/ 
            ret=splice(0, NULL, pipefd[1], NULL, 32768, SPLICE_F_MORE | SPLICE_F_MOVE);
            ret=splice(pipefd[0], NULL, sockfd, NULL, 32768, SPLICE_F_MORE | SPLICE_F_MOVE); 
        }
    }
    

    return 0;
}