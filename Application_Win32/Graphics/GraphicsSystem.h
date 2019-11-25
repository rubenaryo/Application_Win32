/*----------------------------------------------
Ruben Young (rubenaryo@gmail.com)
Date : 2019/10
Description : Interface for the GraphicsSystem class
----------------------------------------------*/
#ifndef GRAPHICSSYSTEM_H
#define GRAPHICSSYSTEM_H

#include "../System/WinApp.h"
#include "../System/GameTimer.h"
#include "../System/IException.h"
#include "D3DClass.h"

namespace System {
namespace Graphics {
    // Global Variables
    // TODO: Create an Options class to hold all Window/UI options for easier interfacing in the future
    const bool FULL_SCREEN   = false;
    const bool VSYNC_ENABLED = true;
    const float SCREEN_DEPTH = 1000.0f;
    const float SCREEN_NEAR  = 0.1f;
    
    class GraphicsSystem
    {
    public:
        GraphicsSystem();
        GraphicsSystem(const GraphicsSystem&);
        ~GraphicsSystem();
    
        bool Init(int a_Width, int a_Height, HWND a_MainWindow);
        void Shutdown();
        bool Frame();
    
    protected:
        bool Render();
        void CalculateFrameStats();

        Direct3DClass* m_pD3DClass;
        GameTimer*     m_pTimer;

    // Custom Graphics Exception
    public:
        class Exception : public IException
        {
        public:
            Exception(int a_Line, const wchar_t* a_Filename, HRESULT a_HRESULT) noexcept;
            const wchar_t* what16() const noexcept override;
            const wchar_t* GetType() const noexcept;
            static std::wstring TranslateErrorCode(HRESULT a_HRESULT) noexcept;
            HRESULT GetErrorCode() const noexcept;
            std::wstring GetErrorString() const noexcept;
        private:
            HRESULT m_HRESULT;
        };
    };
}}

// Custom Exception helper macros
#define GFX_EXCEPT(a_HRESULT) GraphicsSystem::Exception( __LINE__, __WFILE__, a_HRESULT )

#endif