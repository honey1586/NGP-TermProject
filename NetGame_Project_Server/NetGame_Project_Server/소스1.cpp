#define SERVER_PORT 3500
#define MAX_CLNT       2
////////////////////////
#define KEY_LEFT      '0'
#define KEY_RIGHT     '1'
#define KEY_SPACE     '2'
/////////////////////////
#define READY         '3'
#define GAMEOVER      '4'
/////////////////////////
#define BULLET_HIT    '5'
#define MOSNTER_HIT   '6'



#define _WINSOCK_DEPRECATED_NO_WARNINGS // 최신 VC++ 컴파일 시 경고 방지
#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include<iostream>
#include <process.h>
using namespace std;

#pragma pack(push,1)
struct KEY {
    short id;
    char cKey;
};
#pragma pack(pop)

#pragma pack(push,1)
struct Monster {
    short id;
    short x;
    short y;
    RECT rc;
};
#pragma pack(pop)

#pragma pack(push,1)
struct CHero {
    short id;
    short x;
    RECT rc;
};
#pragma pack(pop)

#pragma pack(push,1)
struct CHeroBullet {
    short id;
    short x;
    short y;
    RECT rc;
};
#pragma pack(pop)

#pragma pack(push,1)
struct Boss {
    short id;
    short x;
    short y;
    RECT rc;
};
#pragma pack(pop)

#pragma pack(push,1)
struct BossBullet {
    short id;
    short x;
    short y;
    RECT rc;
};
#pragma pack(pop)

#pragma pack(push,1)
struct HP {
    short id;
    short x;
    short y;
};
#pragma pack(pop)

int main() 
{
    KEY keyInfo{};
    CHero hero[2];
    CHero Bullet[2];
    Boss boss;
    Boss boss_bullet;

}