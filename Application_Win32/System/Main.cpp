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
    SysClass* System = new SysClass();
    
    if (System->Init())
    {
        System->Run();
    }
    
    // Clean up internal systems
    System->Shutdown();
    
    delete System;
    System = nullptr;
    
    return -1;
}