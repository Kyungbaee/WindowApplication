#define WIN32_LEAN_AND_MEAN
#define _CRTDBG_MAP_ALLOC       // 메모리 누수를 탐지하기 위해
#include <stdlib.h>             // 메모리 누수를 탐지하기 위해
#include <crtdbg.h>             // 메모리 누수를 탐지하기 위해
#include <Windows.h>
#include "graphics.h"

// 함수 프로토타입
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
bool CreateMainWindow(HWND &, HINSTANCE, int);
LRESULT WINAPI WinProc(HWND, UINT, WPARAM, LPARAM);

// 전역 변수
HINSTANCE hinst;
// HDC hdc;
//TCHAR ch = ' ';
// RECT rect;
//PAINTSTRUCT ps;
//bool vkKeys[256];


// Graphics 포인터
Graphics *graphics;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // 디버그 모드로 빌드 시 메모리 누수를 확인한다.
    #if defined(DEBUG) | defined(_DEBUG)
        _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    #endif

    MSG msg;
    HWND hwnd = NULL;

    if (!CreateMainWindow(hwnd, hInstance, nCmdShow))
        return 1;

    try {
        // Graphics 객체 생성
        graphics = new Graphics;
        // Graphics 객체 초기화
        graphics->initialize(hwnd, GAME_WIDTH, GAME_HEIGHT, FULLSCREEN);


        // vitual key 배열 초기화
        //for (int i = 0; i < 256; i++)
        //    vkKeys[i] = false;

        int done = 0;
        while (!done)
        {
            if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
            {
                if (msg.message == WM_QUIT)
                    done = 1;

                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
            else
                graphics->showBackbuffer();
        }
        safeDelete(graphics);  // 종료 전에 메모리를 해제
        return msg.wParam;
    }
    catch (const GameError &err)
    {
        MessageBoxA(NULL, err.getMessage(), "error", MB_OK); // 멀티바이트 문자열을 반환하므로 MessageBoxA 함수 사용
    }
    catch (...) {
        MessageBox(NULL, L"Unknwon error occurred in game.", L"Error", MB_OK);
    }
    safeDelete(graphics);  // 종료 전에 메모리를 해제
    return 0;
}

LRESULT WINAPI WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    /*
    short nVirtKey;
    const short SHIFTED = (short)0x8000;
    TEXTMETRIC tm;
    DWORD chWidth = 20;
    DWORD chHeight = 20;
    */
    switch (msg) {
    /*
    case WM_CREATE:
        hdc = GetDC(hWnd);
        GetTextMetrics(hdc, &tm);
        ReleaseDC(hWnd, hdc);
        chWidth = tm.tmAveCharWidth;
        chHeight = tm.tmHeight;
        return 0;*/
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    /*
    case WM_CHAR:
        switch (wParam) {
        case 0x08: // 백스페이스
        case 0x09: // 탭
        case 0x0A: // 라인피드
        case 0x0D: // 캐리지 리턴
        case 0x1B: // 이스케이프
            MessageBeep((UINT)-1); // 비프음이 들리지만 표시 X
            return 0;
        default:    // 표시할 수 있는 문자
            ch = (TCHAR)wParam;  // 문자를 가져
            InvalidateRect(hWnd, NULL, TRUE);
            return 0;
        }
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        TextOut(hdc, 0, 0, &ch, 1);

        // Display the state of vkKeys array
        // Display 'T' if key is down and 'F' is key is up
        for (int r = 0; r < 16; r++)
        {
            for (int c = 0; c < 16; c++)
            {
                if (vkKeys[r * 16 + c])
                {
                    SetBkMode(hdc, OPAQUE);         // opaque text background
                    TextOut(hdc, c * chWidth + chWidth * 2, r * chHeight + chHeight * 2, L"T ", 2);
                }
                else {
                    SetBkMode(hdc, TRANSPARENT);    // transparent text background
                    TextOut(hdc, c * chWidth + chWidth * 2, r * chHeight + chHeight * 2, L"F ", 2);
                }
            }
        }
        EndPaint(hWnd, &ps);
        return 0;
    case WM_KEYDOWN:
        vkKeys[wParam] = true;
        switch (wParam) {
        case VK_SHIFT:
            nVirtKey = GetKeyState(VK_LSHIFT);
            if (nVirtKey & SHIFTED)
                vkKeys[VK_LSHIFT] = true;
            nVirtKey = GetKeyState(VK_RSHIFT);
            if (nVirtKey & SHIFTED)
                vkKeys[VK_RSHIFT] = true;
            break;
        case VK_CONTROL:
            nVirtKey = GetKeyState(VK_LCONTROL);
            if (nVirtKey & SHIFTED)
                vkKeys[VK_LCONTROL] = true;
            nVirtKey = GetKeyState(VK_RCONTROL);
            if (nVirtKey & SHIFTED)
                vkKeys[VK_RCONTROL] = true;
            break;
        }
        InvalidateRect(hWnd, NULL, TRUE);
        return 0;
        break;
    case WM_KEYUP:
        vkKeys[wParam] = false;
        switch (wParam) {
        case VK_SHIFT:
            nVirtKey = GetKeyState(VK_LSHIFT);
            if ((nVirtKey & SHIFTED) == 0)
                vkKeys[VK_LSHIFT] = false;
            nVirtKey = GetKeyState(VK_RSHIFT);
            if ((nVirtKey & SHIFTED) == 0)
                vkKeys[VK_RSHIFT] = false;
            break;
        case VK_CONTROL:
            nVirtKey = GetKeyState(VK_LCONTROL);
            if ((nVirtKey & SHIFTED) == 0)
                vkKeys[VK_LCONTROL] = false;
            nVirtKey = GetKeyState(VK_RCONTROL);
            if ((nVirtKey & SHIFTED) == 0)
                vkKeys[VK_RCONTROL] = false;
            break;
        }
        InvalidateRect(hWnd, NULL, TRUE);
        return 0;
        break;*/
    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
}

bool CreateMainWindow(HWND &hwnd, HINSTANCE hInstance, int nCmdShow) {
    
    // Window 클래스 구조체를 메인 윈도우에 대한 매개변수로 채움
    WNDCLASSEX wcx;
    // HWND hwnd;

    wcx.cbSize = sizeof(wcx);       // 구조체의 크기
    wcx.style = CS_HREDRAW | CS_VREDRAW;        // 크기가 변하면 다시 그림
    wcx.lpfnWndProc = WinProc;      // 윈도우 프리시저를 가리킴
    wcx.cbClsExtra = 0;     // 여분의 클래스 메모리
    wcx.cbWndExtra = 0;     // 여분의 윈도우 메모리
    wcx.hInstance = hInstance;      // 인스턴스의 핸들
    wcx.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcx.hCursor = LoadCursor(NULL, IDC_ARROW);      // 미리 정의된 화살표
    wcx.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);        // 검은색 배경
    wcx.lpszMenuName = NULL;        // 메뉴 리소스의 이름
    wcx.lpszClassName = CLASS_NAME;     // 윈도우 클래스의 이름
    wcx.hIconSm = LoadIcon(NULL, IDI_APPLICATION);      //작은 클래스 이름

    if (RegisterClassEx(&wcx) == 0)
        return false;

    hwnd = CreateWindow(
        CLASS_NAME,     // 윈도우 클래스의 이름
        GAME_TITLE,      // 제목 표시줄의 텍스트
        WS_OVERLAPPEDWINDOW,    //윈도우 스타일
        CW_USEDEFAULT,      // 윈도우의 기본 수평 위치
        CW_USEDEFAULT,      // 윈도우의 기본 수직 위치
        GAME_WIDTH,       // 윈도우의 폭
        GAME_HEIGHT,      // 윈도우의 높이
        (HWND)NULL,         // 부모 윈도우
        (HMENU)NULL,        // 메뉴 없음
        hInstance,          // 애플리케이션 인스턴스의 핸들
        (LPVOID)NULL        // 윈도우 매개변수 없음
    );

    if (!hwnd)
        return false;

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);
    return true;
}

// 뮤텍스 사용
bool AnotherInstance() {
    HANDLE ourMutex;
    ourMutex = CreateMutex(NULL, true, L"Use_a_different_string_here_for_each_program_48161-XYZZY");

    if (GetLastError() == ERROR_ALREADY_EXISTS)
        return true;
    return false;
}