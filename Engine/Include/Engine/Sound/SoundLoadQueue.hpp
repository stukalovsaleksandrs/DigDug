#ifndef ENGINE_QUEUE
#define ENGINE_QUEUE

// Engine
#include "ISoundService.hpp"
// Standard
#include <functional>
#include <future>
#include <queue>

namespace Engine
{
    using Callback = std::function<void()>;
    class SoundLoadQueue final
    {
    public:
        void Push(Callback);
        bool WaitPop(Callback& outLoadCallback);

    private:
        std::queue<Callback> m_queue;

    };
}

#endif// ENGINE_QUEUE
