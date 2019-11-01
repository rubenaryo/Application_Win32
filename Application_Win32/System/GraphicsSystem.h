/*----------------------------------------------
Ruben Young (rubenaryo@gmail.com)
Date : 2019/10
Description : Interface for the GraphicsSystem class
----------------------------------------------*/
#ifndef GRAPHICSSYSTEM_H
#define GRAPHICSSYSTEM_H

#include <Windows.h>

namespace System {
namespace Graphics {
    // Global Variables
    // TODO: Create an Options class to hold all Window/UI options for easier interfacing in the future
    const bool FULL_SCREEN = false;
    const bool VSYNC_ENABLED = true;
    const float SCREEN_DEPTH = 1000.0f;
    const float SCREEN_NEAR = 0.1f;
    
    class GraphicsSystem
    {
    public:
        GraphicsSystem();
        GraphicsSystem(const GraphicsSystem&);
        ~GraphicsSystem();
    
        bool Init(int a_Width, int a_Height, HWND);
        void Shutdown();
        bool Frame();
    
    private:
        bool Render();
    };
}}
#endif