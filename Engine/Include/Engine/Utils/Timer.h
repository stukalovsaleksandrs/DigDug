#ifndef SE_TIMER_H
#define SE_TIMER_H

// Engine
#include "Engine/Core/Singleton.h"

namespace Engine {
    class Timer final : public Singleton<Timer> {
    public:
        [[nodiscard]] float GetFPS() const noexcept { return 1.f / m_deltaSec; };
        [[nodiscard]] float GetDeltaSec() const noexcept { return m_deltaSec; };

        void Update();

    private:
        // The number of seconds passed since the last frame
        float m_deltaSec{};

    };
}

#endif// SE_TIMER_H
