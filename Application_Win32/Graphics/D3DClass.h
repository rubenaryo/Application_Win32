/*----------------------------------------------
Ruben Young (rubenaryo@gmail.com)
Date : 2019/11
Description : Interface for using Direct3D 11
----------------------------------------------*/
#ifndef D3DCLASS_H
#define D3DCLASS_H

// Link Direct3D Libraries
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

// DirectX necessary includes
#include <d3d11.h>
#include <directxmath.h>

namespace System {
namespace Graphics {

class Direct3DClass
{
public:
    Direct3DClass();
    ~Direct3DClass();

    bool Init(int a_Width, int a_Height, HWND a_MainWindow);
    void OnResize();
    void UpdateWindowState();
    void Update(float dt);
    void Draw();

protected:
    void CalculateFrameStats();

    HRESULT CreateDeviceAndContext(UINT a_CreateDeviceFlags, D3D_FEATURE_LEVEL* a_FeatureLevel);

    HRESULT CreateSwapChainAndDepthBuffer();

protected:
    // Client Width, Height, and HWND (Window Handle)
    int m_ClientWidth;
    int m_ClientHeight;
    HWND m_MainWindow;

    // States of the window
    bool m_Paused;
    bool m_Minimized;
    bool m_Maximized;
    bool m_ResizeDragging;

    // Direct3D-Specific fields
    bool m_Enable4xMSAA;
    unsigned int m_4xMSAAQuality;

    ID3D11Device*           m_pD3DDevice;
    ID3D11DeviceContext*    m_pD3DImmediateContext;
    IDXGISwapChain*         m_pSwapChain;
    ID3D11Texture2D*        m_pDepthStencilBuffer;
    ID3D11RenderTargetView* m_pRenderTargetView;
    ID3D11DepthStencilView* m_pDepthStencilView;
    D3D11_VIEWPORT          m_ScreenViewport;
    D3D_DRIVER_TYPE         m_D3DDriverType;

};

}
}

#endif