/*----------------------------------------------
Ruben Young (rubenaryo@gmail.com)
Date : 2019/10
Description : This file contains the main function (entry point) for the application
----------------------------------------------*/

#include <Windows.h>
#include <tchar.h>

#ifndef UNICODE
#define UNICODE
#endif

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

// Entry point for the application
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
    // Register the window class
    const TCHAR CLASS_NAME[] = TEXT("APP_NAME");

    // Declare an empty WNDCLASSEX
    WNDCLASSEX wc;

    // Fill in WNDCLASSEX fields
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = 0;
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = CLASS_NAME;
    wc.hIconSm = LoadIcon(hInstance, IDI_APPLICATION);

    // Register the window and check for failure to register
    if (!RegisterClassEx(&wc)) 
    // Failed:
    { MessageBox(NULL, TEXT("Window Registration Failed!"), TEXT("Oh no!"), MB_ICONEXCLAMATION | MB_OK); exit(0); return 1; }

    // Finally, create a window
    HWND WindowHandle = CreateWindowEx(
        WS_EX_APPWINDOW | WS_EX_WINDOWEDGE, // Optional window styles.
        CLASS_NAME,                         // Window class
        TEXT("Hello World!"),               // Window text
        WS_OVERLAPPEDWINDOW,                // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, 300, 200,

        NULL,                               // Parent window
        NULL,                               // Menu
        hInstance,                          // Instance handle
        NULL                                // Additional application data
    );

    // Check for failure to create window
    if (!WindowHandle) { MessageBox(NULL, TEXT("Create Window Failed!"), TEXT("Oh no!"), MB_ICONEXCLAMATION | MB_OK); exit(0); return 1; }

    // Display the window
    ShowWindow(WindowHandle, nCmdShow);
    UpdateWindow(WindowHandle);

    // Create a message loop
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int) msg.wParam;
}