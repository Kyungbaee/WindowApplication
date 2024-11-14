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

    // Direct3D �ʱ�ȭ
    direct3d = Direct3DCreate9(D3D_SDK_VERSION);
    if (direct3d == NULL)
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing Direct3D"));

    initD3Dpp();        // D3D ���������̼� �Ű����� �ʱ�ȭ

    // determine if graphics card supports harware texturing and lighting and vertex shaders
    D3DCAPS9 caps;
    DWORD behavior;
    result = direct3d->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);
    // If device doesn't support HW T&L or doesn't support 1.1 vertex 
    // shaders in hardware, then switch to software vertex processing.
    if( (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) == 0 ||
            caps.VertexShaderVersion < D3DVS_VERSION(1,1) )
        behavior = D3DCREATE_SOFTWARE_VERTEXPROCESSING;  // �ý��� CPU���� ����Ʈ��� ���� ���� ó��
    else
        behavior = D3DCREATE_HARDWARE_VERTEXPROCESSING;  // �׷��� �ϵ����� ����ó�� ����

    // Direct3D ����̽� ����
    result = direct3d->CreateDevice(
        D3DADAPTER_DEFAULT, // ����� ���÷��� ��� ��
        D3DDEVTYPE_HAL, // ����̽� Ÿ�� : �ϵ���� ������ ��ȯ(�׷��� ���¸� �ȼ��� ��ȯ)�� ����
        hwnd,   // ���ø����̼��� ���׶��� -> ��׶��� ���� ��ȯ���� �˸�
        behavior,   // ���� ó�� �÷��� ����
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
        ZeroMemory(&d3dpp, sizeof(d3dpp));              // ����ü�� 0���� ä��
        // �ʿ��� �Ķ���� ����
        d3dpp.BackBufferWidth   = width;
        d3dpp.BackBufferHeight  = height;
        if(fullscreen)                                  // ��üȭ�� �����̶��,
            d3dpp.BackBufferFormat  = D3DFMT_X8R8G8B8;  // 24 ��Ʈ ����
        else
            d3dpp.BackBufferFormat  = D3DFMT_UNKNOWN;   // ����ũ�� ���� ���
        d3dpp.BackBufferCount   = 1;
        d3dpp.SwapEffect        = D3DSWAPEFFECT_DISCARD;  // ȭ�鿡 �� �۹��� �����ְ� ���� ��, ������ ȭ�� ���� ������ ����
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
// Display the backbuffer(ȭ�鿡 ������ �ʴ� ����)
//=============================================================================
HRESULT Graphics::showBackbuffer()
{
    result = E_FAIL;    // default to fail, replace on success
    // (this function will be moved in later versions)
    // Clear the backbuffer to lime green 
    // ���÷��� ���� ���� ����
    // Clear(Count : pRects�� ����Ű�� �ִ� �迭�� ������ �ִ� ���� �簢���� ����,  -> pRects�� null �̸� 0
    //       pRects : ���� �簢���� ����ϴ� D3DRECT ����ü �迭�� ����Ű�� ������, -> ���� ��ü�� ���� �ʹٸ� null
    //       Flags : ��� ǥ���� ���� ����, -> ���۸� ���� ���ϸ� D3DCLEAR_TARGET ���
    //       Color : ����� ĥ�� RGB ���� ��,
    //       Z : ���� ���۸� ä��� ���� 0~1 ������ �ε��Ҽ������� �� ����.
    //       Stencil : ���ٽ� ���۸� ä��� ���� 0~2^(n-1) ������ �ε��Ҽ������� ����.
    device3d->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0,255,0), 0.0f, 0);

    // Display backbuffer to screen
    // ������ ��ȯ ����
    // Present(const RECT *PSourceRect : ���� �簢�� ��ü�� �����ֱ� ���Ѵٸ� NULL�� ����,
    //         const RECT* PDestRect : ��� �簢�� ��ü�� ä��� ���� NULL�� ����,
    //         HWND hDestWindowOverride : ��� �������� �ڵ� -> NULL�̸� D3DPRESENT_PARAMETERS�� hWndDeviceWindow ����� ����,
    //         const RGNDATA *pDirtyRegion : ���� ü���� D3DSWAPEFFECT_COPY�� �������� �ʴ� �� NULL��,

    result = device3d->Present(NULL, NULL, NULL, NULL);

    return result;
}

//=============================================================================
// Release all (�޸� ��ȯ)
//=============================================================================
void Graphics::releaseAll()
{
    safeRelease(device3d);
    safeRelease(direct3d);
}

