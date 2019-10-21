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

// Custom Exception Interface
#include "IException.h"
#include <string>
#include <sstream>

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


public:
    class Exception : public IException
    {
    public:
        Exception(int a_Line, const wchar_t* a_Filename, HRESULT a_HRESULT) noexcept;
        const wchar_t* what16() const noexcept override;
        const wchar_t* GetType() const noexcept;
        static std::wstring TranslateErrorCode(HRESULT a_HRESULT) noexcept;
        HRESULT GetErrorCode() const noexcept;
        std::wstring GetErrorString() const noexcept;
    private:
        HRESULT m_HRESULT;
    };
};

// Global Application Handle
static SysClass* AppHandle = NULL;

// Custom Exception helper macros
#define WND_EXCEPT(a_HRESULT) SysClass::Exception( __LINE__, __WFILE__, a_HRESULT )
#define WND_LAST_EXCEPT() WND_EXCEPT(GetLastError())

#endif