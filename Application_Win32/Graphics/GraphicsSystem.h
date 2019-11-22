/*----------------------------------------------
Ruben Young (rubenaryo@gmail.com)
Date : 2019/10
Description : Interface for the GraphicsSystem class
----------------------------------------------*/
#ifndef GRAPHICSSYSTEM_H
#define GRAPHICSSYSTEM_H

#include <Windows.h>
#include "D3DClass.h"
#include "../System/IException.h"
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
    
    private:
        bool Render();

        Direct3DClass* m_pD3DClass;

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
#endif