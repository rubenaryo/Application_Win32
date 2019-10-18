/*----------------------------------------------
Ruben Young (rubenaryo@gmail.com)
Date : 2019/10
Description : Interface (header) for System class
Thanks to http://www.rastertek.com/gl40tut02.html
----------------------------------------------*/
#ifndef SYSCLASS_H
#define SYSCLASS_H

// Speed up compilation by disabling certain includes
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

// Include standard windows headers
#include <Windows.h>
#include <stdlib.h>

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
    SysClass(LPCWSTR a_appName);
    SysClass(const SysClass&);
    ~SysClass();

    bool Init();
    void Shutdown();
    void Run();

    // Message Handling Callback for Windows
    LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);
private:
    bool Frame();
    bool InitWindows(int a_Width, int a_Height);
    void ShutdownWindows();

private:
    // Windows Fields
    LPCWSTR m_appName;
    HINSTANCE m_hInstance;
    HWND m_hwnd;

    ///TODO: Pointers to other app systems  
};

// Pointer-type definition
typedef SysClass* pSysClass;

// Global Application Handle
static pSysClass AppHandle = nullptr;

#endif