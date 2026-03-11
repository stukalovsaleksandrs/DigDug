#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H
#include "Singleton.h"

namespace DAE
{
    namespace Components{ class InputComponent; }

    class InputManager final : public Singleton<InputManager>
    {
    public:
        ///@return Whether the application has to quit
        [[nodiscard]] static bool ProcessInput();

    };

}

#endif
