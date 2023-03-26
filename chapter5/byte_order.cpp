#include <cstdio>
#include <iostream>
#include <bits/socket.h>
using std::cout;
using std::endl;
void byte_order() {
    union
    {
        short value;
        char union_bytes[sizeof(short)];
    }test;
    test.value = 0x0102;
    if (test.union_bytes[0] == 1 && test.union_bytes[1] == 2) {
        cout << "big endian!" << endl;
        // 大端字节序也称网络字节序，给所有接收数据的主机提供了一个
        // 正确解释收到的格式化数据的保证
    } else if (test.union_bytes[0] == 2 && test.union_bytes[1] == 1) {
        cout << "little endian!" << endl;
        // 现代PC大多用小端字节序，小端字节序也被称为主机字节序
    } else {
        cout << "unknown ..." << endl;

    }
    
}

int main() {
    cout << sizeof(short) << endl;
    byte_order();
}

//Linux提供了如下4个函数来完成主机字节序和网络字节序之间的 转换:
//#include<netinet/in.h>
// unsigned long int htonl(unsigned long int hostlong); 
// unsigned short int htons(unsigned short int hostshort); 
// unsigned long int ntohl(unsigned long int netlong); 
// unsigned short int ntohs(unsigned short int netshort);