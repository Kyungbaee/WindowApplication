#define WIN32_LEAN_AND_MEAN
#define _CRTDBG_MAP_ALLOC       // �޸� ������ Ž���ϱ� ����
#include <stdlib.h>             // �޸� ������ Ž���ϱ� ����
#include <crtdbg.h>             // �޸� ������ Ž���ϱ� ����
#include <Windows.h>
#include "graphics.h"

// �Լ� ������Ÿ��
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
bool CreateMainWindow(HWND &, HINSTANCE, int);
LRESULT WINAPI WinProc(HWND, UINT, WPARAM, LPARAM);

// ���� ����
HINSTANCE hinst;
// HDC hdc;
//TCHAR ch = ' ';
// RECT rect;
//PAINTSTRUCT ps;
//bool vkKeys[256];


// Graphics ������
Graphics *graphics;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // ����� ���� ���� �� �޸� ������ Ȯ���Ѵ�.
    #if defined(DEBUG) | defined(_DEBUG)
        _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    #endif

    MSG msg;
    HWND hwnd = NULL;

    if (!CreateMainWindow(hwnd, hInstance, nCmdShow))
        return 1;

    try {
        // Graphics ��ü ����
        graphics = new Graphics;
        // Graphics ��ü �ʱ�ȭ
        graphics->initialize(hwnd, GAME_WIDTH, GAME_HEIGHT, FULLSCREEN);


        // vitual key �迭 �ʱ�ȭ
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
        safeDelete(graphics);  // ���� ���� �޸𸮸� ����
        return msg.wParam;
    }
    catch (const GameError &err)
    {
        MessageBoxA(NULL, err.getMessage(), "error", MB_OK); // ��Ƽ����Ʈ ���ڿ��� ��ȯ�ϹǷ� MessageBoxA �Լ� ���
    }
    catch (...) {
        MessageBox(NULL, L"Unknwon error occurred in game.", L"Error", MB_OK);
    }
    safeDelete(graphics);  // ���� ���� �޸𸮸� ����
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
        case 0x08: // �齺���̽�
        case 0x09: // ��
        case 0x0A: // �����ǵ�
        case 0x0D: // ĳ���� ����
        case 0x1B: // �̽�������
            MessageBeep((UINT)-1); // �������� �鸮���� ǥ�� X
            return 0;
        default:    // ǥ���� �� �ִ� ����
            ch = (TCHAR)wParam;  // ���ڸ� ����
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
    
    // Window Ŭ���� ����ü�� ���� �����쿡 ���� �Ű������� ä��
    WNDCLASSEX wcx;
    // HWND hwnd;

    wcx.cbSize = sizeof(wcx);       // ����ü�� ũ��
    wcx.style = CS_HREDRAW | CS_VREDRAW;        // ũ�Ⱑ ���ϸ� �ٽ� �׸�
    wcx.lpfnWndProc = WinProc;      // ������ ���������� ����Ŵ
    wcx.cbClsExtra = 0;     // ������ Ŭ���� �޸�
    wcx.cbWndExtra = 0;     // ������ ������ �޸�
    wcx.hInstance = hInstance;      // �ν��Ͻ��� �ڵ�
    wcx.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcx.hCursor = LoadCursor(NULL, IDC_ARROW);      // �̸� ���ǵ� ȭ��ǥ
    wcx.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);        // ������ ���
    wcx.lpszMenuName = NULL;        // �޴� ���ҽ��� �̸�
    wcx.lpszClassName = CLASS_NAME;     // ������ Ŭ������ �̸�
    wcx.hIconSm = LoadIcon(NULL, IDI_APPLICATION);      //���� Ŭ���� �̸�

    if (RegisterClassEx(&wcx) == 0)
        return false;

    hwnd = CreateWindow(
        CLASS_NAME,     // ������ Ŭ������ �̸�
        GAME_TITLE,      // ���� ǥ������ �ؽ�Ʈ
        WS_OVERLAPPEDWINDOW,    //������ ��Ÿ��
        CW_USEDEFAULT,      // �������� �⺻ ���� ��ġ
        CW_USEDEFAULT,      // �������� �⺻ ���� ��ġ
        GAME_WIDTH,       // �������� ��
        GAME_HEIGHT,      // �������� ����
        (HWND)NULL,         // �θ� ������
        (HMENU)NULL,        // �޴� ����
        hInstance,          // ���ø����̼� �ν��Ͻ��� �ڵ�
        (LPVOID)NULL        // ������ �Ű����� ����
    );

    if (!hwnd)
        return false;

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);
    return true;
}

// ���ؽ� ���
bool AnotherInstance() {
    HANDLE ourMutex;
    ourMutex = CreateMutex(NULL, true, L"Use_a_different_string_here_for_each_program_48161-XYZZY");

    if (GetLastError() == ERROR_ALREADY_EXISTS)
        return true;
    return false;
}