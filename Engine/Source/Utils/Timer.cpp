#include "Utils/Timer.hpp"
#include <chrono>

void Engine::Timer::Update() {
    // Updating delta
    using clock = std::chrono::steady_clock;
    static auto lastFrameTime{ clock::now() };
    auto const currentFrameTime { clock::now() };
    m_deltaSec = std::chrono::duration<float>(currentFrameTime - lastFrameTime).count();
    lastFrameTime = currentFrameTime;
}
