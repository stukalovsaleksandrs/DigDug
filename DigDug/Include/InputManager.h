#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H
#include "Singleton.h"

namespace DAE
{
    class InputManager final : public Singleton<InputManager>
    {
    public:
        bool ProcessInput();
    };

}

#endif
