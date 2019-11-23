/*----------------------------------------------
Ruben Young (rubenaryo@gmail.com)
Date : 2019/10
Description : GraphicsSystem method definitions
----------------------------------------------*/
#include "GraphicsSystem.h"
#include <sstream>
#include <string>

using namespace System::Graphics;

GraphicsSystem::GraphicsSystem() :
    m_pD3DClass(0)
{
    
}

GraphicsSystem::GraphicsSystem(const GraphicsSystem& other) :
    m_pD3DClass(other.m_pD3DClass)
{
}

GraphicsSystem::~GraphicsSystem()
{
}

bool GraphicsSystem::Init(int a_Width, int a_Height, HWND a_MainWindow)
{
    // Create Space for Direct3D and check for null
    m_pD3DClass = new Direct3DClass();
    if (!m_pD3DClass) return false;
    
    // Initialize the direct3d object
    bool tResult = m_pD3DClass->Init(a_Width, a_Height, a_MainWindow);
    if (!tResult) return false;

    return true;
}

void GraphicsSystem::Shutdown()
{
    // Cleanup D3D allocation
    if (m_pD3DClass) delete m_pD3DClass;
    m_pD3DClass = 0;


    return;
}

bool GraphicsSystem::Frame()
{
    return true;
}

bool GraphicsSystem::Render()
{
    return true;
}

#pragma region Graphics Exception Implementation
/*
    Custom Graphics Exception Implementation
        This exception type is mainly defined for exceptions relating to the graphics system, like failing to initialize Direct3D
*/

GraphicsSystem::Exception::Exception(int a_Line, const wchar_t* a_Filename, HRESULT a_HRESULT) noexcept :
    IException(a_Line, a_Filename),
    m_HRESULT(a_HRESULT)
{}

const wchar_t* GraphicsSystem::Exception::what16() const noexcept
{
    std::wstringstream oss;
    oss << GetType() << std::endl
        << "[Error Code] " << std::endl << GetErrorCode() << std::endl << std::endl
        << "[Description] " << std::endl << GetErrorString() << std::endl
        << GetOriginString() << std::endl;

    m_buf = oss.str();
    return m_buf.c_str();
}

const wchar_t* GraphicsSystem::Exception::GetType() const noexcept
{
    return L"Graphics Exception";
}

std::wstring GraphicsSystem::Exception::TranslateErrorCode(HRESULT a_HRESULT) noexcept
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

    // Once copied, we can free the stack buffer
    LocalFree(pMsgBuf);
    return errorString;
}

HRESULT GraphicsSystem::Exception::GetErrorCode() const noexcept
{
    return m_HRESULT;
}

std::wstring GraphicsSystem::Exception::GetErrorString() const noexcept
{
    std::wstring resized = TranslateErrorCode(m_HRESULT);
    return TranslateErrorCode(m_HRESULT);
}
#pragma endregion