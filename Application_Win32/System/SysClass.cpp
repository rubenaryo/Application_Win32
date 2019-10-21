/*----------------------------------------------
Ruben Young (rubenaryo@gmail.com)
Date : 2019/10
Description : Implementation for System class methods
----------------------------------------------*/
#include "SysClass.h"

// Default constructor
SysClass::SysClass() : m_appName(L"NAME_DEFAULT"), m_hInstance(0), m_hwnd(0)
{
    // Initialize all systems to NULL
    m_pInput = NULL;
    m_pGraphics = NULL;
}

// System Initializer
bool SysClass::Init()
{
    // TODO: Evaluate whether it's worth holding screenwidth/height in some manager class
    int screenWidth = 0;
    int screenHeight = 0;

    try
    {
    // Create the application window and check for errors
    if (!InitWindows(screenWidth, screenHeight)) return false;
    }
    catch (const IException& e)
    {
        MessageBox(NULL, e.what16(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
    }

    // Initialize all systems and check for errors
    m_pInput = new InputSystem();
    if (!m_pInput) return false;

    m_pGraphics = new GraphicsSystem();
    if (!m_pGraphics) return false;

    // Also run internal graphics initialization
    if (!m_pGraphics->Init(screenWidth, screenHeight, m_hwnd)) return false;

    return true;
}

bool SysClass::InitWindows(int& a_Width, int& a_Height)
{
    WNDCLASSEX wc;
    DEVMODE screenSettings;
    int xPos = 0;
    int yPos = 0;

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
    wc.hIcon = NULL;                // TODO: Import our own icon!
    wc.hIconSm = NULL;              // TODO: Import our own icon!
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = m_appName;
    wc.cbSize = sizeof(WNDCLASSEX);

    // Register the window and check for failure to register
    HRESULT hregisterError = RegisterClassEx(&wc);
    if (hregisterError == 0)
    {
        throw WND_EXCEPT(hregisterError);
        exit(hregisterError); return false;
    }

    // TODO:
    a_Width = GetSystemMetrics(SM_CXSCREEN);
    a_Height = GetSystemMetrics(SM_CYSCREEN);

    // Check for fullscreen mode and change settings accordingly
    if (FULL_SCREEN)
    {
        // Clear everything at the DEVMODE's memory location
        memset(&screenSettings, 0, sizeof(screenSettings));

        screenSettings.dmSize = sizeof(screenSettings);         // Size of the structure
        screenSettings.dmPelsWidth = (ULONG)a_Width;          // Pixel Width of the visible screen
        screenSettings.dmPelsHeight = (ULONG)a_Height;         // Pixel Height of the visible screen
        screenSettings.dmBitsPerPel = 32;                       // 32 bits per pixel

        // The fields modified
        screenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

        // Set Windows' display settings to full screen
        ChangeDisplaySettings(&screenSettings, CDS_FULLSCREEN);

        // Move window to top left
        xPos = yPos = 0;
    }
    else
    {
        // Default windowed options:
        // 800x600 window in the center of the screen
        a_Width = 800;
        a_Height = 600;

        xPos = (GetSystemMetrics(SM_CXSCREEN) - a_Width) / 2;
        yPos = (GetSystemMetrics(SM_CYSCREEN) - a_Height) / 2;
    }

    DWORD dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;   // Window Extended Style
    DWORD dwWindowStyle = WS_OVERLAPPEDWINDOW;              // Window Borderless Style

    // Define our window rectangle and do some setup
    RECT rc = { xPos, yPos, xPos + a_Width, yPos + a_Height };
    AdjustWindowRectEx(&rc, dwWindowStyle, FALSE, dwExStyle);

    // Create window and hold onto the window handle
    m_hwnd = CreateWindowEx(
        dwExStyle,          // Extended Style
        m_appName,          // Class Name
        m_appName,          // Window Name
        dwWindowStyle,      // Window Style
        xPos, yPos,         // X position, Y position
        a_Width, a_Height,  // Width, Height
        NULL, NULL,         // hwndParent, hMenu
        m_hInstance,        // hInstance
        NULL                // Param from CREATESTRUCT / WM_CREATE
    );

    // Register the window and check for failure to register
    if (!m_hwnd)
    {
        throw WND_LAST_EXCEPT();
        exit(0); return false;
    }

    // Display the window
    ShowWindow(m_hwnd, SW_SHOW);
    SetForegroundWindow(m_hwnd);
    SetFocus(m_hwnd);

    // Hide cursor
    //ShowCursor(false);
    return true;
}

void SysClass::ShutdownWindows()
{
    // Show cursor
    //ShowCursor(true);

    // Fix full screen mode
    if (FULL_SCREEN)
    {
        ChangeDisplaySettings(NULL, 0);
    }

    // Destroy the window
    DestroyWindow(m_hwnd);
    m_hwnd = NULL;

    // Unregister the class
    UnregisterClass(m_appName, m_hInstance);
    m_hInstance = NULL;

    // Release AppHandle
    AppHandle = NULL;
}

void SysClass::Run()
{
    // Create a message (game) loop
    MSG msg;
    bool running = true;

    // Clear all message memory space in case of false messages getting sent
    // TODO: possibly unecessary
    ZeroMemory(&msg, sizeof(MSG));

    while (running)
    {
        // Handle general windows messaging
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        // If the quit message has been sent, exit the application
        if (msg.message == WM_QUIT)
        {
            running = false;
        }
        else
        {
            // Do frame processing for all systems and check for failure
            if (!Frame()) running = false;
        }
    }
}

bool SysClass::Frame()
{
    // For now, just call the graphics system's discrete frame function
    // In the future, here we query the input system to process keyboard input

    // Do frame processing for the graphics system
    if (!m_pGraphics->Frame()) return false;

    return true;
}

void SysClass::Shutdown()
{
    // Release all systems
    if (m_pGraphics)
    {
        m_pGraphics->Shutdown();
        delete m_pGraphics;
        m_pGraphics = NULL;
    }

    if (m_pInput)
    {
        // TODO: Input may require a special shutdown also
        delete m_pInput;
        m_pInput = NULL;
    }

    // Release Windows
    ShutdownWindows();
}

// Destructor
SysClass::~SysClass()
{
    // All cleanup handled in "Shutdown()"
}

// Window Procedure for this application
LRESULT CALLBACK WndProc(_In_ HWND hwnd, _In_  UINT uMsg, _In_  WPARAM wParam, _In_ LPARAM lParam)
{
    // Close the window on CLOSE/DESTROY messages, send all other messages to the message handler
    switch (uMsg)
    {
    case WM_CLOSE:
        PostQuitMessage(WM_QUIT);
        break;
    case WM_DESTROY:
        PostQuitMessage(WM_QUIT);
        break;
    default:
        // Send all other messages to the message handler
        return AppHandle->MessageHandler(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

LRESULT CALLBACK SysClass::MessageHandler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    // Handle all special case messages
    switch (uMsg)
    {
    case WM_KEYDOWN: // Key Press Event
        //m_pInput->KeyDown((unsigned int)wparam);
        return 0;
        // Check if a key has been released on the keyboard.
    case WM_KEYUP: // Key Release Event
        //m_Input->KeyUp((unsigned int)wparam);
        return 0;
        // Any other messages send to the default message handler as our application won't make use of them.
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

/*
    Custom Exception Implementation
*/

SysClass::Exception::Exception(int a_Line, const wchar_t* a_Filename, HRESULT a_HRESULT) noexcept 
    : IException(a_Line, a_Filename)
    , m_HRESULT(a_HRESULT)
{}

const wchar_t* SysClass::Exception::what16() const noexcept
{
    std::wstringstream oss;
    oss << GetType() << std::endl
        << "[Error Code] "  << std::endl << GetErrorCode()   << std::endl << std::endl
        << "[Description] " << std::endl << GetErrorString() << std::endl
        << GetOriginString() << std::endl;

    m_buf = oss.str();
    return m_buf.c_str();
}

const wchar_t* SysClass::Exception::GetType() const noexcept
{
    return L"Windows Exception";
}

std::wstring SysClass::Exception::TranslateErrorCode(HRESULT a_HRESULT) noexcept
{
    wchar_t* pMsgBuf = NULL;

    const DWORD nMsgLen = FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, a_HRESULT, 0,
        reinterpret_cast<LPWSTR>(&pMsgBuf), 0, NULL
    );

    // Failure
    if (nMsgLen == 0)
    {
        return L"Unidentified error code";
    }

    std::wstring errorString = pMsgBuf;

    // Once, copied,we can free the stack buffer
    LocalFree(pMsgBuf);
    return errorString;
}

HRESULT SysClass::Exception::GetErrorCode() const noexcept
{
    return m_HRESULT;
}

std::wstring SysClass::Exception::GetErrorString() const noexcept
{
    std::wstring resized = TranslateErrorCode(m_HRESULT);
    return TranslateErrorCode(m_HRESULT);
}
