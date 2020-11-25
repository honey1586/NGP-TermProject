#define _WINSOCK_DEPRECATED_NO_WARNINGS // √÷Ω≈ VC++ ƒƒ∆ƒ¿œ Ω√ ∞Ê∞Ì πÊ¡ˆ
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
#define KEY_SPACE '9'


#define MAX_CLNT 2
DWORD WINAPI Client_Thread(LPVOID arg);
DWORD WINAPI Operation_Thread(LPVOID arg);

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

#pragma region ø¿∑˘ √‚∑¬ ∫Œ∫–
// º“ƒœ «‘ºˆ ø¿∑˘ √‚∑¬ »ƒ ¡æ∑·
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
// º“ƒœ «‘ºˆ ø¿∑˘ √‚∑¬
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
SOCKET clientSocks[MAX_CLNT];//≈¨∂Û¿Ãæ∆Æ º“ƒœ ∫∏∞¸øÎ πËø≠
HANDLE hMutex;//π¬≈ÿΩ∫
HANDLE hReadEvent, hOperEvent;
KEY keyInfo{ KEY_NULL };
CHero hero[2];
HeroBullet hbullet[10];
char buf[BUFSIZE];
HANDLE hThread;
HANDLE hThread2;

//void KeyMessage(const char* key, HeroBullet& hbullet) 
//{
//    if (KEY_SPACE == *key) 
//    {
//        hbullet.y
//    }
//}

void KeyMessage(const char* key, CHero& hero)
{
    if (KEY_RIGHT == *key)
    {
        cout << hero.x << endl;
        hero.x += 5;
    }
    else if (KEY_LEFT == *key)
    {
        cout << hero.x << endl;
        hero.x -= 5;
    }
}

int main(int argc, char* argv[])
{
    // HANDLE hMutex;
    // π¬≈ÿΩ∫

    hMutex = CreateMutex(NULL, FALSE, NULL);//«œ≥™¿« π¬≈ÿΩ∫∏¶ ª˝º∫«—¥Ÿ.

    // ¿Ã∫•∆Æ ª˝º∫
    hReadEvent = CreateEvent(NULL, FALSE, TRUE, NULL);
    if (hReadEvent == NULL) return 1;
    hOperEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    if (hOperEvent == NULL) return 1;

    int retval;
    // ¿©º” √ ±‚»≠
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        return 1;

    printf("º≠πˆsocket()\n");
    // socket()
    SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_sock == INVALID_SOCKET) err_quit("socket()");


    printf("º≠πˆbind()\n");
    // bind()
    SOCKADDR_IN serveraddr;
    ZeroMemory(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(SERVERPORT);
    retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
    if (retval == SOCKET_ERROR) err_quit("bind()");


    printf("º≠πˆlisten()\n");
    // listen()
    retval = listen(listen_sock, SOMAXCONN);
    if (retval == SOCKET_ERROR) err_quit("listen()");

    // º≠πˆ ¥Î±‚ ªÛ≈¬ øœ∑· -----------------------------------------
    printf("º≠πˆ ¥Î±‚ ªÛ≈¬ øœ∑·\n");


    // µ•¿Ã≈Õ ≈ÎΩ≈ø° ªÁøÎ«“ ∫Øºˆ
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

        /*WaitForSingleObject(hMutex, INFINITE);*/
        clientCount++;
        clientSocks[clientCount] = client_sock;
        //hero[clientCount].connect = true;
        //hero[clientCount].id = clientCount;
        hero[clientCount] = CHero{ 0,true,(short)clientCount,NULL };
        hero[clientCount].id = clientCount;
        send(clientSocks[clientCount], (char*)&hero[clientCount], sizeof(CHero), 0);

        cout << "¡¢º”«— ≈¨∂Û ∞≥ºˆ : " << clientCount << endl;
        cout << "hero [" << clientCount << "].id : " << hero[clientCount].id << endl;
        /*ReleaseMutex(hMutex);*/

        hThread = CreateThread(NULL, 0, Client_Thread, (LPVOID)&client_sock, 0, NULL);//HandleClient æ≤∑πµÂ Ω««‡, clientSock¿ª ∏≈∞≥∫Øºˆ∑Œ ¿¸¥ﬁ
        printf("Connected Client IP : %s\n", inet_ntoa(clientaddr.sin_addr));

    }

    // closesocket()
    closesocket(listen_sock);

    // ¿©º” ¡æ∑·
    WSACleanup();
    return 0;
}

DWORD WINAPI Client_Thread(LPVOID arg)
{
    SOCKET clientSock = *((SOCKET*)arg); //∏≈∞≥∫Øºˆ∑Œπﬁ¿∫ ≈¨∂Û¿Ãæ∆Æ º“ƒœ¿ª ¿¸¥ﬁ

    int retval;
    int i = 0;


    while (true) {
        WaitForSingleObject(hReadEvent, INFINITE);
        hThread2 = CreateThread(NULL, 0, Operation_Thread, (LPVOID)&clientSock, 0, NULL);
        recv(clientSock, (char*)&keyInfo, sizeof(KEY), 0);

    }
        SetEvent(hOperEvent);

    closesocket(clientSock);//º“ƒœ¿ª ¡æ∑·«—¥Ÿ.
    return 0;
}

DWORD WINAPI Operation_Thread(LPVOID arg)
{
    SOCKET clientSock = *((SOCKET*)arg); //∏≈∞≥∫Øºˆ∑Œπﬁ¿∫ ≈¨∂Û¿Ãæ∆Æ º“ƒœ¿ª ¿¸¥ﬁ
    WaitForSingleObject(hOperEvent, INFINITE);
    memcpy(buf, (char*)&hero, sizeof(hero));
    for (int i = 0; i < 2; ++i) {

<<<<<<< HEAD
    KeyMessage(&keyInfo.cKey, hero[keyInfo.id]);

    send(clientSock, buf, sizeof(hero), 0);
=======
>>>>>>> parent of ee82caf... ÌîåÎ†àÏù¥Ïñ¥ÎßàÎã§ Í∞ÅÏûê Ï¥ùÏïå ÏÉùÏÑ±
        if (hero[i].id == 0) {
            KeyMessage(&keyInfo.cKey, hero[keyInfo.id]);
        }

        if (hero[i].id == 1) {
            KeyMessage(&keyInfo.cKey, hero[keyInfo.id]);
        }
    }
    SetEvent(hReadEvent);
    return 0;
}