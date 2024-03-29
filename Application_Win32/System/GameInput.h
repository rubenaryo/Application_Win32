/*----------------------------------------------
Ruben Young (rubenaryo@gmail.com)
Date : 2019/11
Description : Interface for GameInput child class
----------------------------------------------*/
#ifndef GAMEINPUT_H
#define GAMEINPUT_H

#include "InputSystem.h"

namespace System {
namespace Input {
    
    class GameInput : public InputSystem
    {
    public:
        // Default constructor
        GameInput();

        // Frame method
        void Frame();
    
    protected:
        // Override implementation for setting default key mappings
        virtual void SetDefaultKeyMap() override;
    };
}
}
#endif