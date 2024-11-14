// Programming 2D Games
// Copyright (c) 2011 by:
// Charles Kelly
// Chapter 3 graphics.cpp v1.2
// Last modification: March-6-2015

#include "graphics.h"

//=============================================================================
// Constructor
//=============================================================================
Graphics::Graphics()
{
    direct3d = NULL;
    device3d = NULL;
    fullscreen = false;
    width = GAME_WIDTH;    // width & height are replaced in initialize()
    height = GAME_HEIGHT;
}

//=============================================================================
// Destructor
//=============================================================================
Graphics::~Graphics()
{
    releaseAll();
}

//=============================================================================
// Initialize DirectX graphics
// throws GameError on error
//=============================================================================
void Graphics::initialize(HWND hw, int w, int h, bool full)
{
    hwnd = hw;
    width = w;
    height = h;
    fullscreen = full;

    // Direct3D 초기화
    direct3d = Direct3DCreate9(D3D_SDK_VERSION);
    if (direct3d == NULL)
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing Direct3D"));

    initD3Dpp();        // D3D 프레젠테이션 매개변수 초기화

    // determine if graphics card supports harware texturing and lighting and vertex shaders
    D3DCAPS9 caps;
    DWORD behavior;
    result = direct3d->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);
    // If device doesn't support HW T&L or doesn't support 1.1 vertex 
    // shaders in hardware, then switch to software vertex processing.
    if( (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) == 0 ||
            caps.VertexShaderVersion < D3DVS_VERSION(1,1) )
        behavior = D3DCREATE_SOFTWARE_VERTEXPROCESSING;  // 시스템 CPU에서 소프트웨어를 통해 정점 처리
    else
        behavior = D3DCREATE_HARDWARE_VERTEXPROCESSING;  // 그래픽 하드웨어에서 정점처리 수행

    // Direct3D 디바이스 생성
    result = direct3d->CreateDevice(
        D3DADAPTER_DEFAULT, // 사용할 디스플레이 어뎁터 수
        D3DDEVTYPE_HAL, // 디바이스 타입 : 하드웨어 레스터 변환(그래픽 형태를 픽셀로 변환)을 지정
        hwnd,   // 애플리케이션이 포그라운드 -> 백그라운드 모드로 전환됨을 알림
        behavior,   // 정점 처리 플래그 적용
        &d3dpp, 
        &device3d);

    if (FAILED(result))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error creating Direct3D device"));
 
}

//=============================================================================
// Initialize D3D presentation parameters
//=============================================================================
void Graphics::initD3Dpp()
{
    try{
        ZeroMemory(&d3dpp, sizeof(d3dpp));              // 구조체를 0으로 채움
        // 필요한 파라미터 세팅
        d3dpp.BackBufferWidth   = width;
        d3dpp.BackBufferHeight  = height;
        if(fullscreen)                                  // 전체화면 구성이라면,
            d3dpp.BackBufferFormat  = D3DFMT_X8R8G8B8;  // 24 비트 색상
        else
            d3dpp.BackBufferFormat  = D3DFMT_UNKNOWN;   // 데스크톱 설정 사용
        d3dpp.BackBufferCount   = 1;
        d3dpp.SwapEffect        = D3DSWAPEFFECT_DISCARD;  // 화면에 백 퍼버를 보여주고 있을 때, 이전에 화면 버퍼 내용은 삭제
        d3dpp.hDeviceWindow     = hwnd;
        d3dpp.Windowed          = (!fullscreen);
        d3dpp.PresentationInterval   = D3DPRESENT_INTERVAL_IMMEDIATE;
    } catch(...)
    {
        throw(GameError(gameErrorNS::FATAL_ERROR, 
                "Error initializing D3D presentation parameters"));

    }
}

//=============================================================================
// Display the backbuffer(화면에 보이지 않는 버퍼)
//=============================================================================
HRESULT Graphics::showBackbuffer()
{
    result = E_FAIL;    // default to fail, replace on success
    // (this function will be moved in later versions)
    // Clear the backbuffer to lime green 
    // 디스플레이 버퍼 비우기 수행
    // Clear(Count : pRects가 가리키고 있는 배열에 나열돼 있는 지울 사각형의 개수,  -> pRects가 null 이면 0
    //       pRects : 지울 사각형을 기술하는 D3DRECT 구조체 배열을 가리키는 포인터, -> 버퍼 전체를 비우고 싶다면 null
    //       Flags : 비울 표현의 형식 지정, -> 버퍼를 비우길 원하면 D3DCLEAR_TARGET 사용
    //       Color : 대상을 칠할 RGB 색상 값,
    //       Z : 깊이 버퍼를 채우기 위해 0~1 사이의 부동소수점으로 값 지정.
    //       Stencil : 스텐실 버퍼를 채우기 위해 0~2^(n-1) 사이의 부동소수점으로 지정.
    device3d->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0,255,0), 0.0f, 0);

    // Display backbuffer to screen
    // 페이지 전환 수행
    // Present(const RECT *PSourceRect : 원본 사각형 전체를 보여주기 위한다면 NULL로 설정,
    //         const RECT* PDestRect : 대상 사각형 전체를 채우기 위해 NULL로 설정,
    //         HWND hDestWindowOverride : 대상 윈도우의 핸들 -> NULL이면 D3DPRESENT_PARAMETERS의 hWndDeviceWindow 멤버가 사용됨,
    //         const RGNDATA *pDirtyRegion : 스왑 체인이 D3DSWAPEFFECT_COPY로 생성되지 않는 한 NULL임,

    result = device3d->Present(NULL, NULL, NULL, NULL);

    return result;
}

//=============================================================================
// Release all (메모리 반환)
//=============================================================================
void Graphics::releaseAll()
{
    safeRelease(device3d);
    safeRelease(direct3d);
}

