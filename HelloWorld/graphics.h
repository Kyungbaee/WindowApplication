// Programming 2D Games
// Copyright (c) 2011 by:
// Charles Kelly
// Chapter 3 graphics.h v1.1
// Last modification: March-6-2015

#ifndef _GRAPHICS_H             // Prevent multiple definitions if this (여러 곳에서 이 파일을 포함하는 경우)
#define _GRAPHICS_H             // file is included in more than one place (다중 정의를 방지함)
#define WIN32_LEAN_AND_MEAN

#ifdef _DEBUG
#define D3D_DEBUG_INFO
#endif

class Graphics;

#include <d3d9.h>
#include "constants.h"
#include "gameError.h"

// DirectX pointer types
#define LP_3DDEVICE LPDIRECT3DDEVICE9
#define LP_3D       LPDIRECT3D9

// Color defines
#define COLOR_ARGB DWORD
#define SETCOLOR_ARGB(a,r,g,b) \
    ((COLOR_ARGB)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)))

class Graphics
{
private:
    // DirectX pointers and stuff
    LP_3D       direct3d;
    LP_3DDEVICE device3d;
    D3DPRESENT_PARAMETERS d3dpp;

    // other variables
    HRESULT     result;         // standard Windows return codes
    HWND        hwnd;
    bool        fullscreen;
    int         width;
    int         height;

    // (For internal engine use only. No user serviceable parts inside.)
    // Initialize D3D presentation parameters
    void    initD3Dpp();

public:
    // Constructor
    Graphics();

    // Destructor
    virtual ~Graphics();

    // Releases direct3d and device3d.
    void    releaseAll();

    // Initialize DirectX graphics
    // Throws GameError on error
    // Pre: hw = handle to window
    //      width = width in pixels
    //      height = height in pixels
    //      fullscreen = true for full screen, false for window
    void    initialize(HWND hw, int width, int height, bool fullscreen);

    // Display the offscreen backbuffer to the screen.
    HRESULT showBackbuffer();
};

#endif

