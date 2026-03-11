#ifndef INPUTCOMPONENT_H
#define INPUTCOMPONENT_H
#include "ComponentBase.h"

namespace DAE::Components
{
    class InputComponent final : public Component
    {
    public:
        explicit InputComponent(GameObject& owner) noexcept;
        void Update() noexcept override;
    };

}

#endif // INPUTCOMPONENT_H
