#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

// 함수 프로토타입
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
bool CreateMainWindow(HINSTANCE, int);
LRESULT WINAPI WinProc(HWND, UINT, WPARAM, LPARAM);

// 전역 변수
HINSTANCE hinst;
HDC hdc;
TCHAR ch = ' ';
RECT rect;
PAINTSTRUCT ps;
bool vkKeys[256];

// 상수
const wchar_t CLASS_NAME[] = L"WinMain";
const wchar_t APP_TITLE[] = L"Hello World";
const int WINDOW_WIDTH = 400;
const int WINDOW_HEIGHT = 300;


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    MSG msg;

    if (!CreateMainWindow(hInstance, nCmdShow))
        return false;

    // vitual key 배열 초기화
    for (int i = 0; i < 256; i++)
        vkKeys[i] = false;

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
    }
    return msg.wParam;
}

LRESULT WINAPI WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    short nVirtKey;
    const short SHIFTED = (short)0x8000;
    TEXTMETRIC tm;
    DWORD chWidth = 20;
    DWORD chHeight = 20;

    switch (msg) {
    case WM_CREATE:
        hdc = GetDC(hWnd);
        GetTextMetrics(hdc, &tm);
        ReleaseDC(hWnd, hdc);
        chWidth = tm.tmAveCharWidth;
        chHeight = tm.tmHeight;
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
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
        break;
    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
}

bool CreateMainWindow(HINSTANCE hInstance, int nCmdShow) {
    WNDCLASSEX wcx;
    HWND hwnd;

    wcx.cbSize = sizeof(wcx);
    wcx.style = CS_HREDRAW | CS_VREDRAW;
    wcx.lpfnWndProc = WinProc;
    wcx.cbClsExtra = 0;
    wcx.cbWndExtra = 0;
    wcx.hInstance = hInstance;
    wcx.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcx.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcx.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcx.lpszMenuName = NULL;
    wcx.lpszClassName = CLASS_NAME;
    wcx.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (RegisterClassEx(&wcx) == 0)
        return false;

    hwnd = CreateWindow(
        CLASS_NAME,
        APP_TITLE,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        (HWND)NULL,
        (HMENU)NULL,
        hInstance,
        (LPVOID)NULL
    );

    if (!hwnd)
        return false;

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);
    return true;
}
