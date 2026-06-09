#ifndef DIGDUG_AI_COMPONENT
#define DIGDUG_AI_COMPONENT

// Engine
#include "Engine/Commands.hpp"
#include "Engine/Components/ComponentBase.hpp"

namespace Game
{
    class AIComponent final : public Engine::Component
    {
    public:
        explicit AIComponent(Engine::GameObject& owner);

    private:


    };
}

#endif// DIGDUG_AI_COMPONENT