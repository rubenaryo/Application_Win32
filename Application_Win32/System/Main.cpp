/*----------------------------------------------
Ruben Young (rubenaryo@gmail.com)
Date : 2019/10
Description : This file contains the main function (entry point) for the application
----------------------------------------------*/
#include "SysClass.h"

using namespace System;

// Entry point for the application
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_  LPWSTR lpCmdLine, _In_  int nCmdShow)
{
    // On Debug Builds: Enable Runtime memory check
    #if defined(DEBUG) | defined(_DEBUG)
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    #endif 

    SysClass* System = new SysClass();
    
    if (System->Init())
    {
        System->Run();
    }
    else
        return -1;
    
    // Clean up internal systems
    System->Shutdown();
    
    delete System;
    System = nullptr;
    
    return 0;
}