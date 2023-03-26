//Linux提供了如下4个函数来完成主机字节序和网络字节序之间的 转换:
//#include<netinet/in.h>
// unsigned long int htonl(unsigned long int hostlong); 
// unsigned short int htons(unsigned short int hostshort); 
// unsigned long int ntohl(unsigned long int netlong); 
// unsigned short int ntohs(unsigned short int netshort);

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
using std::cout;
using std::endl;
// 点分十进制字符串表示的IPv4地址和用网络字节序 整数表示的IPv4地址之间的转换
// in_addr_t inet_addr(const char*strptr);
// int inet_aton(const char*cp,struct in_addr*inp); 
// char*inet_ntoa(struct in_addr in);

int main() {
    struct in_addr in1, in2;
    int trans1 = inet_aton("1.2.3.4", &in1);
    int trans2 = inet_aton("6.2.3.4", &in2);
    cout << in1.s_addr << endl;
    if (trans1) {
        char* szValue1 = inet_ntoa(in1);
        cout << szValue1 << endl;
    }
    if (trans2) {
        char* szValue2 = inet_ntoa(in2);
        cout << szValue2 << endl;


    }
    return 0;
    // cout << szValue1 << endl;
}