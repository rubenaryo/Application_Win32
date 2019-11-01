/*----------------------------------------------
Ruben Young (rubenaryo@gmail.com)
Date : 2019/11
Description : Implementation of GameInput method overrides
----------------------------------------------*/
#include "SysClass.h"
#include "GameInput.h"
#include "InputSystem.h"

namespace System {
namespace Input {

    GameInput::GameInput()
    {
        SetDefaultKeyMap();
    }

    void GameInput::SetDefaultKeyMap()
    {
        m_keyMap.clear();
        m_keyMap[GameCommands::Quit] = new Chord(L"Quit", VK_ESCAPE, KeyState::JustReleased);
    }

    void GameInput::Frame()
    {
        GetInput();

        // Act on user input:
        // - Iterate through all active keys
        // - Check for commands corresponding to activated chords
        // - Do something based on those commands
        for (auto pair : m_activeKeyMap)
        {
            switch (pair.first)
            {
            case GameCommands::Quit:
                //PostQuitMessage(0);
                break;
            }
        }
    }
}
}