#include <winsock2.h>
#include <iostream>
#include <vector>
#include <string>
#include <typeinfo>
#pragma comment(lib, "Ws2_32.lib")
#pragma warning(disable : 4996)

template <typename T>
struct tlv_struct
{
    unsigned short int tag;
    int16_t len;
    T value;
};

int main()
{
    WSADATA WSAData;
    if (WSAStartup(MAKEWORD(1, 1), &WSAData) != 0)
    {
        std::cout << "WSAStartup faild. Error:" << WSAGetLastError();
        return FALSE;
    }

    SOCKET s;

    if (INVALID_SOCKET == (s = socket(AF_INET, SOCK_STREAM, 0)))
    {
        std::cout << WSAGetLastError();
        system("pause");
    }

    struct sockaddr_in sin;
    sin.sin_family = AF_INET;
    std::cout << "Enter port: ";
    int port;
    std::cin >> port;
    sin.sin_port = htons(port);
    sin.sin_addr.s_addr = INADDR_ANY;

    int err = bind(s, (LPSOCKADDR)&sin, sizeof(sin));
    err = listen(s, 100);

    while (true)
    {
        sockaddr_in from1;
        int fromlen1 = sizeof(from1);
        SOCKET s1 = accept(s, (struct sockaddr *)&from1, &fromlen1);

        std::cout << "---------------------" << std::endl;
        tlv_struct<char[100]> tlv1;
        char data1[sizeof(tlv1)];
        recv(s1, data1, sizeof(tlv1), 0);

        memcpy(&tlv1, data1, sizeof(tlv1));
        std::cout << "0x" << std::hex << tlv1.tag << std::endl;
        std::cout << tlv1.len << std::endl;
        std::cout << tlv1.value << std::endl;
        
        
        tlv_struct<char[4]>answer;
        answer.tag = 0x1111;
        strcpy(answer.value, "OK");
        answer.len = strlen("OK");
        char charAnswer[sizeof(answer)];
        memcpy(charAnswer,&answer, sizeof(charAnswer));
        int iSendResult = send(s1, charAnswer, sizeof(charAnswer), 0);
        if (iSendResult == SOCKET_ERROR)
        {
            printf("send failed: %d\n", WSAGetLastError());
            closesocket(s);
            WSACleanup();
            return 1;
        }
        else
        {
            tlv_struct<char[2]>answer;
            answer.tag = 0x1111;

            strcpy(answer.value, "OK");
            answer.len = strlen("OK");

            std::cout << "Success\n";
        }

        try
        {
            std::cout << "---------------------" << std::endl;
            tlv_struct<char[100]> tlv;
            char data2[sizeof(tlv)];
            sockaddr_in from2;
            int fromlen2 = sizeof(from2);
            SOCKET s3 = accept(s, (struct sockaddr *)&from2, &fromlen2);
            recv(s3, data2, sizeof(tlv), 0);

            memcpy(&tlv, data2, sizeof(tlv));
            std::cout << "0x" << std::hex << tlv.tag << std::endl;
            std::cout << tlv.len << std::endl;
            std::cout << tlv.value << std::endl;
        }
        catch (const std::exception &e)
        {
            std::cout << "------FALI----\n";
        }

        // char test1[9] = "testing1";
        // int iSendResult1 =  send(s1, test1, sizeof(test1), 0);
        // if (iSendResult1 == SOCKET_ERROR)
        // {
        //     printf("send failed: %d\n", WSAGetLastError());
        //     closesocket(s);
        //     WSACleanup();
        //     return 1;
        // }
        // else
        // {
        //    std::cout << iSendResult1;
        // }

        // std::cout << "---------------------" << std::endl;
        // tlv_struct<int>tlv2;
        // char data[sizeof(tlv2)];
        // sockaddr_in from;
        // int fromlen = sizeof(from);
        // SOCKET s2 = accept(s, (struct sockaddr*)&from, &fromlen);
        // recv(s2, data, sizeof(tlv2), 0);

        // memcpy(&tlv2, data, sizeof(tlv2));
        // std::cout << "0x" << std::hex << tlv2.tag << std::endl;
        // std::cout << tlv2.len << std::endl;
        // std::cout << std::dec << tlv2.value << std::endl;
    }
    // closesocket(s1);
    WSACleanup();
    system("pause");
    return 0;
}