#ifndef HEALTH_COMPONENT_H
#define HEALTH_COMPONENT_H

#include "ComponentBase.h"

namespace DAE::Components
{
    class LivesComponent : Component
    {
    public:
        explicit LivesComponent(GameObject& owner, uint32_t lives = 2);

        void OnLivesChanged(int deltaLives);

    private:
        uint32_t m_lives{};

    };
}

#endif
