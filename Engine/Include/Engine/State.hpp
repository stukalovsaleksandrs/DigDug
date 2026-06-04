#ifndef ENGINE_STATE
#define ENGINE_STATE

// Standard
#include <memory>

namespace Engine
{
    class IState;
    using pState = std::unique_ptr<IState>;
    class IState
    {
    public:
        virtual ~IState() = default;
        virtual pState Update() noexcept = 0;
        virtual void OnEnter() noexcept = 0;
        virtual void OnExit() noexcept = 0;
    };
}

#endif
