#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <cstdlib>
#include <cassert>
#include <cerrno>
#include <cstdio>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    if (argc <= 2) {
        printf("usage %s ip_address port_number\n", argv[0]);
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

    int ret = connect(sock, (struct sockaddr*)&address, sizeof address);

    std::string data;
    char buf[BUFSIZ];
    memset(buf, '\0', sizeof buf);
    if (ret != -1) {
        while(1) {

            const char* normal_data = "abc";
            const char* oob_data = "123";
            std::cout << ">>> ";
            std::cin >> data;
            int data_send = send(sock, data.c_str(), data.size(), 0);
            assert(data_send != -1);
            // sleep(1);
            // int response = recv(sock, buf, sizeof buf, 0);
            // printf("%s\n", buf);
            // memset(buf, '\0', sizeof buf);

            // data_send = send(sock, oob_data, strlen(oob_data), MSG_OOB);
            // assert(data_send != -1);
            // sleep(1);

            // data_send = send(sock, normal_data, strlen(normal_data), 0);
            // assert(data_send != -1);
            // sleep(1);
        }



    } else {
        printf("connection failed!\n");
    }
    close(sock);
    return 0;
}