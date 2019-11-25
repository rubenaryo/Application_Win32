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
    m_ScreenViewport({0}),
    m_D3DDriverType(D3D_DRIVER_TYPE_HARDWARE)
{}

Direct3DClass::~Direct3DClass()
{}

bool Direct3DClass::Init(int a_Width, int a_Height, HWND a_MainWindow)
{
    // Store Width, Height, HWND as Member Fields
    m_ClientWidth  = a_Width;
    m_ClientHeight = a_Height; 
    m_MainWindow   = a_MainWindow;

    // Device Creation Flags (Either null or debug for now)
    UINT CreateDeviceFlags = 0U;
    #if defined(DEBUG) || defined(_DEBUG)
    CreateDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
    #endif

    // Passed upon creating device and context to store the supported feature levels
    D3D_FEATURE_LEVEL featureLevel;

    // HRESULT holds return values of initialization wrapper functions
    HRESULT tProcessResult;

    // Call wrapper function to create the device
    tProcessResult = CreateDeviceAndContext(CreateDeviceFlags, &featureLevel);

    // Check for failure to create device or unsupported feature level
    if (FAILED(tProcessResult)) { throw GFX_EXCEPT(tProcessResult); return false; }
    if (featureLevel != D3D_FEATURE_LEVEL_11_0) { MessageBox(0, L"Direct3D Feature Level 11 unsupported.", 0, 0); return false; }

    // Check 4xMSAA Quality
    m_pD3DDevice->CheckMultisampleQualityLevels( DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m_4xMSAAQuality );
    assert(m_4xMSAAQuality > 0U); // The returned quantity should always be greater than 0

    // Describe and Create Swap Chain
    tProcessResult = CreateSwapChainAndDepthBuffer();
    
    // Check for failure to create swap chain
    if (FAILED(tProcessResult)) { throw GFX_EXCEPT(tProcessResult); return false; }

    // Create Render Target View
    ID3D11Texture2D* pBackBuffer = 0;
    m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
        reinterpret_cast<void**>(&pBackBuffer));

    if (pBackBuffer)
        m_pD3DDevice->CreateRenderTargetView(
            pBackBuffer,                        // What resource to use as the render target
            0, &m_pRenderTargetView);           // Create a view to the first mip level (default when param 2 == null) 
    else
        return false;

    // Bind views to OM Stage
    m_pD3DImmediateContext->OMSetRenderTargets(
        1, &m_pRenderTargetView, m_pDepthStencilView);

    // Describe and Set Viewport
    m_ScreenViewport.TopLeftX = 0.f;
    m_ScreenViewport.TopLeftY = 0.f;
    m_ScreenViewport.Width = static_cast<FLOAT>(m_ClientWidth);
    m_ScreenViewport.Height = static_cast<FLOAT>(m_ClientHeight);
    m_ScreenViewport.MinDepth = 0.f;
    m_ScreenViewport.MaxDepth = 1.f;

    m_pD3DImmediateContext->RSSetViewports(1, &m_ScreenViewport);

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

HRESULT Direct3DClass::CreateSwapChainAndDepthBuffer()
{
#pragma region Fill Out Swap Chain and Depth Buffer Descriptions
    DXGI_SWAP_CHAIN_DESC SCdesc;
    SCdesc.BufferDesc.Width                     = m_ClientWidth;
    SCdesc.BufferDesc.Height                    = m_ClientHeight;
    SCdesc.BufferDesc.RefreshRate.Numerator     = 60;
    SCdesc.BufferDesc.RefreshRate.Denominator   = 1;
    SCdesc.BufferDesc.Format                    = DXGI_FORMAT_R8G8B8A8_UNORM;
    SCdesc.BufferDesc.ScanlineOrdering          = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    SCdesc.BufferDesc.Scaling                   = DXGI_MODE_SCALING_UNSPECIFIED;
    SCdesc.BufferUsage                          = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    SCdesc.BufferCount                          = 1;                        // Needs to be >= 2 for Flip Model
    SCdesc.OutputWindow                         = m_MainWindow;
    SCdesc.Windowed                             = true;
    SCdesc.SwapEffect                           = DXGI_SWAP_EFFECT_DISCARD; //TODO: Switch to Flip-Model to avoid deprecation
    SCdesc.Flags                                = 0U;

    D3D11_TEXTURE2D_DESC depthStencilDesc;
    depthStencilDesc.Width              = m_ClientWidth;
    depthStencilDesc.Height             = m_ClientHeight;
    depthStencilDesc.MipLevels          = 1;
    depthStencilDesc.ArraySize          = 1;
    depthStencilDesc.Format             = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilDesc.Usage              = D3D11_USAGE_DEFAULT;
    depthStencilDesc.BindFlags          = D3D11_BIND_DEPTH_STENCIL;
    depthStencilDesc.CPUAccessFlags     = 0;
    depthStencilDesc.MiscFlags          = 0;

    // Check for AA
    if (m_Enable4xMSAA)
    {
        SCdesc.SampleDesc.Count = 4;
        SCdesc.SampleDesc.Quality = m_4xMSAAQuality - 1;

        depthStencilDesc.SampleDesc.Count = 4;
        depthStencilDesc.SampleDesc.Quality = m_4xMSAAQuality - 1;
    }
    else // No MSAA
    {
        SCdesc.SampleDesc.Count = 1;
        SCdesc.SampleDesc.Quality = 0;

        depthStencilDesc.SampleDesc.Count = 1;
        depthStencilDesc.SampleDesc.Quality = 0;
    }
#pragma endregion

    // Grab Necessary instance of IDXGIFactory
    IDXGIDevice* dxgiDevice = 0;
    m_pD3DDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)& dxgiDevice);
    IDXGIAdapter* dxgiAdapter = 0;
    dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)& dxgiAdapter);
    IDXGIFactory* dxgiFactory = 0;
    dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)& dxgiFactory);

    // Finally, create the Swap Chain itself (stored in m_pSwapChain)
    HRESULT tProcessResult;
    tProcessResult = dxgiFactory->CreateSwapChain(
        m_pD3DDevice,
        &SCdesc,
        &m_pSwapChain
    );

    // Check for Swap Chain Creation Failure
    if (FAILED(tProcessResult)) 
    { 
        // Release COM interfaces
        dxgiDevice->Release();
        dxgiAdapter->Release();
        dxgiFactory->Release();
        return tProcessResult; 
    }

    // Create Depth/Stencil Buffer
    tProcessResult = m_pD3DDevice->CreateTexture2D(     
        &depthStencilDesc,
        0,
        &m_pDepthStencilBuffer);

    // Check for Texture Creation failure
    if (FAILED(tProcessResult)) { return tProcessResult; }

    tProcessResult = m_pD3DDevice->CreateDepthStencilView(
        m_pDepthStencilBuffer,
        0,                    // Use first mipmap level
        &m_pDepthStencilView);

    // Check for View Creation failure
    if (FAILED(tProcessResult)) { return tProcessResult; }

    // Release COM interfaces (TODO: Verify what should be done in case of swap chain creation failure)
    dxgiDevice->Release();
    dxgiAdapter->Release();
    dxgiFactory->Release();

    // Return successful HRESULT
    return tProcessResult;
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
