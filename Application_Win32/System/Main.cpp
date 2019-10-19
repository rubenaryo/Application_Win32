/*----------------------------------------------
Ruben Young (rubenaryo@gmail.com)
Date : 2019/10
Description : This file contains the main function (entry point) for the application
----------------------------------------------*/
#include "SysClass.h"

// Entry point for the application
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
    pSysClass System = new SysClass(L"Hello World!");

    if (System->Init())
    {
        System->Run();
    }

    // Clean up internal systems
    System->Shutdown();

    delete System;
    System = nullptr;

    return 0;
}

