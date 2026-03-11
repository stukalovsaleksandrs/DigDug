#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H
#include "Singleton.h"

namespace DAE::Input
{
   class Command;

    enum class GamepadButtons
    {

    };

    class InputManager final : public Singleton<InputManager>
    {
    public:
        ///@return Whether the application has to quit
        [[nodiscard]] static bool ProcessInput();

    private:
        Input::Command* m_buttonWest{};
    };

}

#endif
