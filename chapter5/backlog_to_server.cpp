#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <unistd.h>
#include <cstdlib>
#include <cassert>
#include <cstdio>
#include <cstring>

static bool stop = false;
static void handle_term(int sig) {
    stop = true;
}

int main(int argc, char* argv[]) {
    signal(SIGTERM, handle_term);
    if (argc <= 3) {
        printf("usage:%s ip_address port_number backlog\n",basename(argv[0]));
        return 1;
    }
    const char* ip = argv[1];
    int port = atoi(argv[2]);

    //允许该服务器接受的最大连接数
    int backlog = atoi(argv[3]);
    int sock = socket(PF_INET, SOCK_STREAM, 0);
    assert(sock > 0);
    struct sockaddr_in address;

    bzero(&address, sizeof address);
    address.sin_family = AF_INET;

    //inet_pton -- presentation to network?
    inet_pton(AF_INET, ip, &address.sin_addr);

    // htons -- host to network short?
    address.sin_port = htons(port);

    int ret = bind(sock,(struct sockaddr*)&address, sizeof address);

    assert(ret != 1);

    ret = listen(sock, backlog);
    while (!stop) {
        sleep(1);
    }
    close(sock);

    return 0;

}