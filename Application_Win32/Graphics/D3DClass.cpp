/*----------------------------------------------
Ruben Young (rubenaryo@gmail.com)
Date : 2019/11
Description : Implementation of base Direct3D functionality
----------------------------------------------*/
#include "GraphicsSystem.h" //GFX_EXCEPT
#include "D3DClass.h"
using namespace System::Graphics;

// Default Constructor Initializes all member fields to 0
Direct3DClass::Direct3DClass() :
    m_ClientWidth(0),
    m_ClientHeight(0),
    m_MainWindow(0),
    m_Paused(false),
    m_Minimized(false),
    m_Maximized(false),
    m_ResizeDragging(false),
    m_Enable4xMSAA(false),
    m_4xMSAAQuality(0U),
    m_pD3DDevice(0),
    m_pD3DImmediateContext(0),
    m_pSwapChain(0),
    m_pDepthStencilBuffer(0),
    m_pRenderTargetView(0),
    m_pDepthStencilView(0),
    m_ScreenViewport({}),
    m_D3DDriverType(D3D_DRIVER_TYPE_HARDWARE)
{}

Direct3DClass::~Direct3DClass()
{}

bool Direct3DClass::Init(int a_Width, int a_Height, HWND a_MainWindow)
{
    // Device Creation Flags (Either null or debug for now)
    UINT CreateDeviceFlags = 0U;
    #if defined(DEBUG) || defined(_DEBUG)
    CreateDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
    #endif

    // Passed upon creating device and context to store the supported feature levels
    D3D_FEATURE_LEVEL featureLevel;

    // Call wrapper function to create the device
    HRESULT hr = CreateDeviceAndContext(CreateDeviceFlags, &featureLevel);

    // Check for failure to create device or unsupported feature level
    if (FAILED(hr)) { throw GFX_EXCEPT(hr); return false; }
    if (featureLevel != D3D_FEATURE_LEVEL_11_0) { MessageBox(0, L"Direct3D Feature Level 11 unsupported.", 0, 0); return false; }

    // Check 4xMSAA Quality
    m_pD3DDevice->CheckMultisampleQualityLevels( DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m_4xMSAAQuality );
    assert(m_4xMSAAQuality > 0U); // The returned quantity should always be greater than 0



    return true;
}

// Wrapper Method for first step of initialization
// Done like this for learning reasons and readability
HRESULT Direct3DClass::CreateDeviceAndContext(UINT a_CreateDeviceFlags, D3D_FEATURE_LEVEL* a_FeatureLevel)
{
    return D3D11CreateDevice(
        0,                        // pAdapter (IDXGIAdapter*): Using default adapter 
        D3D_DRIVER_TYPE_HARDWARE, // DriverType (D3D_DRIVER_TYPE): Always hardware for now
        0,                        // Software Driver (HMODULE): N/A
        a_CreateDeviceFlags,      // Device Creation Flags (UINT) : Debug flag enabled for debug build, 0 otherwise
        0, 0,                     // Use default feature level array
        D3D11_SDK_VERSION,        // SDK Version (UINT) : Always D3D11_SDK_VERSION
        &m_pD3DDevice,            // ID3D11Device** : Pass device pointer to member field
        a_FeatureLevel,           // D3D_FEATURE_LEVEL* : Return first supported feature level through parameter
        &m_pD3DImmediateContext   // ID3D11DeviceContext** : Pass created device context to member field
    );
}

void Direct3DClass::OnResize()
{    
}    
     
void Direct3DClass::UpdateWindowState()
{    
}    
     
void Direct3DClass::Update(float dt)
{    
}    
     
void Direct3DClass::Draw()
{    
}    
     
void Direct3DClass::CalculateFrameStats()
{
}


