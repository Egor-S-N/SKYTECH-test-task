#include <winsock2.h>
#include <iostream>
#include<iostream>
#include <stdio.h>

#include <typeinfo>
#include <vector>
# include <string.h>

template<typename T>
struct tlv_struct
{
    unsigned short int tag;
    int16_t len;
    T value;
    
};
template<typename T>
void SendStruct(tlv_struct<T> struc, SOCKET s)
{
    char frame[sizeof(struc)];
    memcpy(frame,&struc, sizeof(struc));
    send(s, frame, sizeof(frame), 0);   
}




int main()
{
    WSADATA WSAData;
    if (WSAStartup(MAKEWORD(1, 1), &WSAData) != 0)
    {
        std::cout << "WSAStartup faild. Error:" << WSAGetLastError();
        return FALSE;
    }

    SOCKET s1 = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in anAddr;
    anAddr.sin_family = AF_INET;
    std::cout << "Enter port: ";
    int port;
    std::cin >> port;
    anAddr.sin_port = htons(port);
    std::cout << "Enter IP: ";
    char ip[100];
    std::cin >> ip;//127.0.0.1

    anAddr.sin_addr.S_un.S_addr = inet_addr(ip);
    connect(s1, (struct sockaddr*)&anAddr, sizeof(struct sockaddr));
    
    // // ОТПРАВКА NAME or SURNAME
    tlv_struct <char[100]> tlv;
    tlv.tag = 0x0303;
    std::cout << "Enter name: ";
    std::string name;
    std::cin >> name;
    strcpy(tlv.value,name.c_str());
    tlv.len = strlen(name.c_str());

   
    
    //ОТПРАВКА ID
    // tlv_struct <int> tlv1;
    // tlv1.tag = 0x2222;
    // std::cout << "Enter id: ";
    // int id;
    // std::cin >> id;
    // tlv1.value = id;
    // std::string temp = std::to_string(id);

    // tlv1.len = temp.length();
    // std::cout << "\nlen: " << tlv1.len;


    SendStruct(tlv,s1);
        sockaddr_in from1;
        int fromlen1 = sizeof(from1);
        SOCKET sAnswer = accept(s1, (struct sockaddr *)&from1, &fromlen1);
        tlv_struct<char[4]> answerStruct;
        char data1[sizeof(answerStruct)];
        recv(s1, data1, sizeof(answerStruct), 0);

        memcpy(&answerStruct, data1, sizeof(answerStruct));
        std::cout  << "FROM SERVER: "<< answerStruct.value << std::endl;

    

    closesocket(s1);
    WSACleanup();
    system("pause");
    return 0;
}


