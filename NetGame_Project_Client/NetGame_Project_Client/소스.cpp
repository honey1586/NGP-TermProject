#include <iostream>
#include <Windows.h>
#include <tchar.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <conio.h>
#include <atlimage.h>   // CImage ≈¨∑°Ω∫∞° ¡§¿«µ» «Ï¥ı

using namespace std;
#pragma comment (lib,"WS2_32.LIB")

#define SERVERPORT 9000
#define SERVERIP "127.0.0.1"
#define BUFSIZE 1024

#define KEY_NULL '0'
#define KEY_DOWN '2'
#define KEY_LEFT '4'
#define KEY_RIGHT '6'
#define KEY_UP '8'
<<<<<<< HEAD
<<<<<<< HEAD

#define bulletMax 10
=======
>>>>>>> parent of ee82caf... ÌîåÎ†àÏù¥Ïñ¥ÎßàÎã§ Í∞ÅÏûê Ï¥ùÏïå ÏÉùÏÑ±
=======
>>>>>>> parent of ee82caf... ÌîåÎ†àÏù¥Ïñ¥ÎßàÎã§ Í∞ÅÏûê Ï¥ùÏïå ÏÉùÏÑ±

int Window_Size_X = 460;
int Window_Size_Y = 614;

HINSTANCE g_hInst;
LPCTSTR lpszClass = "Window Class Name";
LPCTSTR lpszWindowName = "NGP";
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
    HWND hWnd;
    MSG Message;
    WNDCLASSEX WndClass;
    g_hInst = hInstance;

    WndClass.cbSize = sizeof(WndClass);
    WndClass.style = CS_HREDRAW | CS_VREDRAW;
    WndClass.lpfnWndProc = (WNDPROC)WndProc;
    WndClass.cbClsExtra = 0;
    WndClass.cbWndExtra = 0;
    WndClass.hInstance = hInstance;
    WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    WndClass.lpszMenuName = NULL;
    WndClass.lpszClassName = lpszClass;
    WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    RegisterClassEx(&WndClass);

    hWnd = CreateWindow

    (lpszClass,
        lpszWindowName,
        (WS_OVERLAPPEDWINDOW),
        0,
        0,
        Window_Size_X,
        Window_Size_Y,
        NULL,
        (HMENU)NULL,
        hInstance,
        NULL);

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);


    while (GetMessage(&Message, 0, 0, 0)) {
        TranslateMessage(&Message);
        DispatchMessage(&Message);
    }
    return Message.wParam;
}

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

//#pragma pack(push,1)
//struct HeroBullet {
//    short x;
//    short y;
//    short id;
//    CImage img;
//    RECT rc;
//};
//#pragma pack(pop)
class Point {
public:
    int x;
    int y;

    Point() {};
    Point(int _x, int _y) : x(_x), y(_y) {}
    //Point(int _x, int _y) : x(_x), y(_y) {}
};
class CBullet {
public:
    CImage img;
    Point pt;
    bool isActivated;
    RECT rc;
    int size = 64;

    CBullet() {
        setPos(Point(0, 614));
    }
    ~CBullet() {}

    void UnActivated() {
        isActivated = false;
        setPos(Point(0, 614));
    }

    void setPos(Point input) {
        pt = input;
        rc = RECT{
             pt.x - size / 2
            ,pt.y - size / 2
            ,pt.x + size / 2
            ,pt.y + size / 2
        };
    }
};


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


CHero hero[2];
static KEY keyInfo{ KEY_NULL };    // ¿‘∑¬µ» ≈∞ ¡§∫∏ ±∏¡∂√º
static bool SockConnect = false;
static bool MyRect = false;
CImage imgBackGround;
CImage imgBackBuff;
CImage heroimg;
CImage heroimg2;
bool leftMove = false;
bool rightMove = false;

CBullet hbullet[bulletMax];

int BulletSpawnTick = 0;

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
#pragma endregion ø¿∑˘ √‚∑¬ ∫Œ∫–

<<<<<<< HEAD
<<<<<<< HEAD

=======
>>>>>>> parent of ee82caf... ÌîåÎ†àÏù¥Ïñ¥ÎßàÎã§ Í∞ÅÏûê Ï¥ùÏïå ÏÉùÏÑ±
=======
>>>>>>> parent of ee82caf... ÌîåÎ†àÏù¥Ïñ¥ÎßàÎã§ Í∞ÅÏûê Ï¥ùÏïå ÏÉùÏÑ±
void ImgLoad() {
    // BG img load
    imgBackGround.Load(TEXT("BG.png"));

    // Hero img load
    heroimg.Load(TEXT("hero.png"));
    heroimg2.Load(TEXT("hero2.png"));
<<<<<<< HEAD
<<<<<<< HEAD

    for (int i = 0; i < bulletMax; ++i) {
        hbullet[i].img.Load(TEXT("bullet.png"));
    }
=======
>>>>>>> parent of ee82caf... ÌîåÎ†àÏù¥Ïñ¥ÎßàÎã§ Í∞ÅÏûê Ï¥ùÏïå ÏÉùÏÑ±
=======
>>>>>>> parent of ee82caf... ÌîåÎ†àÏù¥Ïñ¥ÎßàÎã§ Í∞ÅÏûê Ï¥ùÏïå ÏÉùÏÑ±
}

void OnDraw(HWND hWnd)
{
    // ¿©µµøÏ ¿¸√º ≈©±‚
    RECT rcClient;
    GetClientRect(hWnd, &rcClient);
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hWnd, &ps);
    HDC memDC = imgBackBuff.GetDC();

    // √ ±‚»≠
    FillRect(memDC, &rcClient, static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH)));

    //BG
    imgBackGround.Draw(memDC, 0, 0, 460, 614);

    // hero draw
    if (true == MyRect)
    {
        for (int i = 0; i < 2; ++i)
        {
            if (true == hero[i].connect)
            {
                if (keyInfo.id == i)
                {
                    heroimg.Draw(memDC, hero[i].x, 460, 90, 90);
                }
                else
                {
                    heroimg2.Draw(memDC, hero[i].x, 460, 90, 90);
                }
            }
        }
    }
    // bullet draw
    for (int i = 0; i < bulletMax; ++i) {
        hbullet[i].img.Draw(memDC, hbullet[i].pt.x, hbullet[i].pt.y,hbullet[i].size, hbullet[i].size);
    }

    SetBkMode(memDC, TRANSPARENT);

    imgBackBuff.Draw(hdc, 0, 0);
    imgBackBuff.ReleaseDC();
    EndPaint(hWnd, &ps);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;

    static char messageBuffer[BUFSIZE];

    static SOCKET sock;
    switch (message)
    {
#pragma region √ ±‚»≠

    case WM_CREATE:
    {
        // send, recv «‘ºˆ √‚∑¬∞™ ¿˙¿ÂøÎ
        int retval;

        // ¿©º” √ ±‚»≠
        WSADATA wsa;
        if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
            return 1;

        // socket()
        sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock == INVALID_SOCKET) err_quit("socket()");

        // connect()
        SOCKADDR_IN serveraddr;
        ZeroMemory(&serveraddr, sizeof(serveraddr));
        serveraddr.sin_family = AF_INET;
        serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
        serveraddr.sin_port = htons(SERVERPORT);
        retval = connect(sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
        if (retval == SOCKET_ERROR) err_quit("connect()");

        recv(sock, messageBuffer, sizeof(CHero), 0);
        hero[0] = *(CHero*)messageBuffer;
        keyInfo.id = hero[0].id;

        SockConnect = true;
        MyRect = true;

        // ¿ÃπÃ¡ˆ ∑ŒµÂ
        ImgLoad();

        //// πÈ πˆ∆€ ª˝º∫
        imgBackBuff.Create(Window_Size_X, Window_Size_Y, 24);

        SetTimer(hWnd, 1, 16, NULL);
        SetTimer(hWnd, 2, 16, NULL);
        break;
    }
#pragma endregion

#pragma region PAINT
    case WM_PAINT:
        OnDraw(hWnd);
        break;
#pragma endregion

#pragma region ≈∏¿Ã∏”

    case WM_TIMER:
        switch (wParam)
        {
        case 1:
            if (true == SockConnect)
            {
                send(sock, (char*)&keyInfo, sizeof(KEY), 0);
                recv(sock, (char*)&hero, sizeof(hero), 0);
            }
            break;

        case 2:
            //bullet spawn
            ++BulletSpawnTick;
            if (BulletSpawnTick > 8) {
                for (int i = 0; i < bulletMax; ++i) {
                    if (hbullet[i].isActivated == false) {
                        hbullet[i].isActivated = true;
                        hbullet[i].setPos(Point(hero[0].x + 15, 400));
                        break;
                    }
                }
                BulletSpawnTick = 0;
            }

            //bullet move
            for (int i = 0; i < bulletMax; ++i) {
                if (hbullet[i].isActivated == true) {
                    hbullet[i].setPos(Point(hbullet[i].pt.x, hbullet[i].pt.y - 10));
                }
                if (hbullet[i].pt.y <= -10) {
                    hbullet[i].UnActivated();
                }
            }
            break;
        }
        InvalidateRect(hWnd, NULL, FALSE);
        break;

#pragma endregion

#pragma region ≈∞¿‘∑¬
    case WM_KEYUP:
        if (wParam == VK_RIGHT) {
            keyInfo.cKey = KEY_NULL;
        }
        else if (wParam == VK_LEFT) {
            keyInfo.cKey = KEY_NULL;
        }
        break;

    case WM_KEYFIRST:

        if (wParam == VK_RIGHT)
        {
            keyInfo.cKey = KEY_RIGHT;
        }
        else if (wParam == VK_LEFT)
        {
            keyInfo.cKey = KEY_LEFT;
        }
        InvalidateRect(hWnd, NULL, FALSE); // FALSE∑Œ «œ∏È ¿ÃæÓ¡¸  
        break;
#pragma endregion

    case WM_DESTROY:
        // CImage¥¬ «ÿ¡¶«ÿµµ µ«∞Ì æ» «ÿµµ µ 
        WSACleanup();
        imgBackBuff.Destroy();
        PostQuitMessage(0);
        break;

    default: return DefWindowProc(hWnd, message, wParam, lParam);
    }


    return 0;
}