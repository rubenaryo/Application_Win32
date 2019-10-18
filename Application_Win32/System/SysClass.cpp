/*----------------------------------------------
Ruben Young (rubenaryo@gmail.com)
Date : 2019/10
Description : Implementation for System class methods
----------------------------------------------*/
#include "SysClass.h"

// Default constructor
SysClass::SysClass()
{
    m_appName = L"NAME_DEFAULT";
}

// Named constructor
SysClass::SysClass(LPCWSTR a_appName)
{
    m_appName = a_appName;
}

// Copy constructor
SysClass::SysClass(const SysClass& other)
{

}

// System Initializer
bool SysClass::Init()
{
    int screenWidth = 640;
    int screenHeight = 400;
    bool windowsResult;

    // Create the application window
    windowsResult = InitWindows(screenWidth, screenHeight);

    return true;
}

bool SysClass::InitWindows(int a_Width, int a_Height)
{
    WNDCLASSEX wc;
    DEVMODE screenSettings;
    int xPos, yPos;

    // Global pointer to SysClass object
    AppHandle = this;

    // Grab Instance Handle
    m_hInstance = GetModuleHandle(NULL);

    // Initialize WNDCLASS values
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = m_hInstance;
    wc.hIcon = LoadIcon(m_hInstance, IDI_WINLOGO);
    wc.hIconSm = wc.hIcon;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = m_appName;
    wc.cbSize = sizeof(WNDCLASSEX);

    // Register the window and check for failure to register
    if (!RegisterClassEx(&wc))
    {
        MessageBox(NULL, L"Window Registration Failed!", L"Oh no!", MB_ICONEXCLAMATION | MB_OK);
        exit(0); return false;
    }

    DWORD dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;   // Window Extended Style
    DWORD dwWindowStyle = WS_OVERLAPPEDWINDOW;              // Window Borderless Style

    // Create window and hold onto the window handle
    m_hwnd = CreateWindowEx( 
        dwExStyle,      // Extended Style
        m_appName,      // Class Name
        m_appName,      // Window Name
        dwWindowStyle,  // Window Style
        100, 100,       // xPos, yPos
        a_Width, a_Height,       // width, height
        NULL, NULL,     // hwndParent, hMenu
        m_hInstance,    // hInstance
        NULL            // Param from CREATESTRUCT / WM_CREATE
    );

    // Register the window and check for failure to register
    if (!m_hwnd)
    {
        MessageBox(NULL, L"Create Window Failed!", L"Oh no!", MB_ICONEXCLAMATION | MB_OK);
        exit(0); return false;
    }

    // Display the window
    ShowWindow(m_hwnd, SW_SHOW);
    UpdateWindow(m_hwnd);

    return true;
}

void SysClass::Run()
{
    // Create a message (game) loop
    MSG msg;
    bool running = true;
    while (running)
    {
        // Handle general windows messaging
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        if (msg.message == WM_QUIT)
        {
            running = false;
        }
        else
        {
            // run frames
            m_appName = L"something else";
        }
    }
    return;
}

void SysClass::Shutdown()
{
    return;
}

// Destructor
SysClass::~SysClass()
{
    // All cleanup handled in "Shutdown()"
}

// Window Procedure for this application
LRESULT CALLBACK WndProc(_In_ HWND hwnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
    //Callbacks for winapi 
    switch (uMsg)
    {
    case WM_CLOSE: // if the close message was sent
        DestroyWindow(hwnd); //destroy the window
        break;
    case WM_DESTROY:// if the destroy message was sent
        PostQuitMessage(0); //post the quit message to the message manager
        break;
    case WM_KEYUP:
        //if escape
        if (wParam == VK_ESCAPE)
        {
            PostQuitMessage(0); //post the quit message to the message manager
        }
        break;
    case WM_PAINT: // if we need to repaint the window, paint it white
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
        EndPaint(hwnd, &ps);
        break;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}