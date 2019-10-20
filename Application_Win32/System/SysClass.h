/*----------------------------------------------
Ruben Young (rubenaryo@gmail.com)
Date : 2019/10
Description : Interface (header) for System class
Thanks to http://www.rastertek.com/dx11s2tut02.html
----------------------------------------------*/
#ifndef SYSCLASS_H
#define SYSCLASS_H

// Include standard windows headers
#include "WinApp.h" // Windows.h included here
#include <stdlib.h>

// Other Systems
#include "GraphicsSystem.h"
#include "InputSystem.h"

// Use Unicode character representations
#ifndef UNICODE
#define UNICODE
#endif

// WNDPROC function prototype
static LRESULT CALLBACK WndProc(_In_ HWND, _In_ UINT, _In_ WPARAM, _In_ LPARAM);

// Class definition for System class
class SysClass
{
public:
    SysClass();
    ~SysClass();

    /// Exposed functions for main system operation
    bool Init();
    void Run();
    void Shutdown();

    // Message Handling Callback for Windows
    LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);
private:
    /// Discrete actions that need to be done per frame
    bool Frame();

    /// Initialize main window using Win32 API
    bool InitWindows(int& a_Width, int& a_Height);

    /// Clean up Win32
    void ShutdownWindows();

private:
    // Windows Fields
    LPCWSTR         m_appName;
    HINSTANCE       m_hInstance;
    HWND            m_hwnd;

    // Main Systems
    GraphicsSystem* m_pGraphics;
    InputSystem* m_pInput;
};

// Global Application Handle
static SysClass* AppHandle = NULL;

#endif