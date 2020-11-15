#define _WINSOCK_DEPRECATED_NO_WARNINGS // �ֽ� VC++ ������ �� ��� ����
#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include<iostream>
#include <process.h>
using namespace std;

#define SERVERPORT 9000
#define BUFSIZE 1024

#define KEY_NULL '0'
#define KEY_DOWN '2'
#define KEY_LEFT '4'
#define KEY_RIGHT '6'
#define KEY_UP '8'

#define MAX_CLNT 2
DWORD WINAPI Client_Thread(LPVOID arg);

#pragma pack(push,1)
struct KEY {
    char cKey;
    short id;
};
#pragma pack(pop)

#pragma pack(push,1)
struct CHero {
    short x;
    bool connect;
    short id;
    RECT rc;
};
#pragma pack(pop)

#pragma pack(push,1)
struct Monster {
    short x;
    short y;
    short id;
    RECT rc;
};
#pragma pack(pop)

#pragma pack(push,1)
struct HeroBullet {
    short x;
    short y;
    short id;
    RECT rc;
};
#pragma pack(pop)

#pragma pack(push,1)
struct Boss {
    short x;
    short y;
    short id;
    RECT rc;
};
#pragma pack(pop)

#pragma pack(push,1)
struct BossBullet {
    short x;
    short y;
    short id;
    RECT rc;
};
#pragma pack(pop)

#pragma pack(push,1)
struct HP {
    short x;
    short y;
    short size;
    short id;
};
#pragma pack(pop)

#pragma region ���� ��� �κ�
// ���� �Լ� ���� ��� �� ����
void err_quit(char* msg)
{
    LPVOID lpMsgBuf;
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
        NULL, WSAGetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&lpMsgBuf, 0, NULL);
    MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
    LocalFree(lpMsgBuf);
    exit(1);
}
// ���� �Լ� ���� ���
void err_display(char* msg)
{
    LPVOID lpMsgBuf;
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
        NULL, WSAGetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&lpMsgBuf, 0, NULL);
    printf("[%s] %s", msg, (char*)lpMsgBuf);
    LocalFree(lpMsgBuf);
}
#pragma endregion

int clientCount = -1;
SOCKET clientSocks[MAX_CLNT];//Ŭ���̾�Ʈ ���� ������ �迭
HANDLE hMutex;//���ؽ�
KEY keyInfo{ KEY_NULL };
CHero hero[2];

void KeyMessage(const char* key, CHero& hero)
{
    if (KEY_RIGHT == *key)
    {
        cout << hero.x << endl;
        hero.x += 5;
    }
    if (KEY_LEFT == *key)
    {
        cout << hero.x << endl;
        hero.x -= 5;
    }
}

int main(int argc, char* argv[])
{
    // HANDLE hMutex;
    // ���ؽ�
    HANDLE hThread;
    hMutex = CreateMutex(NULL, FALSE, NULL);//�ϳ��� ���ؽ��� �����Ѵ�.

    int retval;
    // ���� �ʱ�ȭ
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        return 1;

    printf("����socket()\n");
    // socket()
    SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_sock == INVALID_SOCKET) err_quit("socket()");


    printf("����bind()\n");
    // bind()
    SOCKADDR_IN serveraddr;
    ZeroMemory(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(SERVERPORT);
    retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
    if (retval == SOCKET_ERROR) err_quit("bind()");


    printf("����listen()\n");
    // listen()
    retval = listen(listen_sock, SOMAXCONN);
    if (retval == SOCKET_ERROR) err_quit("listen()");

    // ���� ��� ���� �Ϸ� -----------------------------------------
    printf("���� ��� ���� �Ϸ�\n");


    // ������ ��ſ� ����� ����
    SOCKET client_sock;
    SOCKADDR_IN clientaddr;
    int addrlen;

    while (1) {
        //accept()
        addrlen = sizeof(clientaddr);
        client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);
        if (client_sock == INVALID_SOCKET) {
            err_display("accept()");
        }

        WaitForSingleObject(hMutex, INFINITE);
        clientCount++;
        clientSocks[clientCount] = client_sock;
        //hero[clientCount].connect = true;
        //hero[clientCount].id = clientCount;
        hero[clientCount] = CHero{ 0,true,(short)clientCount,NULL };
        hero[clientCount].id = clientCount;
        send(clientSocks[clientCount], (char*)&hero[clientCount], sizeof(CHero), 0);

        cout << "������ Ŭ�� ���� : " << clientCount << endl;
        cout << "hero [" << clientCount << "].id : " << hero[clientCount].id << endl;
        ReleaseMutex(hMutex);

        hThread = CreateThread(NULL, 0, Client_Thread, (LPVOID)&client_sock, 0, NULL);//HandleClient ������ ����, clientSock�� �Ű������� ����
        printf("Connected Client IP : %s\n", inet_ntoa(clientaddr.sin_addr));
    }

    // closesocket()
    closesocket(listen_sock);

    // ���� ����
    WSACleanup();
    return 0;
}

DWORD WINAPI Client_Thread(LPVOID arg)
{

    SOCKET clientSock = *((SOCKET*)arg); //�Ű������ι��� Ŭ���̾�Ʈ ������ ����

    int retval;
    int i = 0;


    while (true) {
        retval = recv(clientSock, (char*)&keyInfo, sizeof(KEY), 0);
        if (retval == SOCKET_ERROR) {
            err_display("recv()");
            exit(1);
        }

        for (int i = 0; i < 2; ++i) {

            if (hero[i].id == 0) {
                KeyMessage(&keyInfo.cKey, hero[keyInfo.id]);
            }

            if (hero[i].id == 1) {
                KeyMessage(&keyInfo.cKey, hero[keyInfo.id]);
            }
        }

        retval = send(clientSock, (char*)&hero, sizeof(hero), 0);

        if (retval == SOCKET_ERROR) {
            err_display("send()");
            exit(1);
        }
    }

    closesocket(clientSock);//������ �����Ѵ�.

    return 0;
}