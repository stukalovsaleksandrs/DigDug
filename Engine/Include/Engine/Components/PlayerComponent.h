#ifndef PLAYER_CONTROLLER_H
#define PLAYER_CONTROLLER_H
#include "ComponentBase.h"
#include "Engine/Core/Observer.h"
#include "Engine/Input/InputManager.h"

namespace DAE::Components
{
    class MovementComponent;
    class PlayerComponent : public Component, public Observer
    {
    public:
        Subject subject;

        explicit PlayerComponent(GameObject& owner) noexcept;
        ~PlayerComponent() noexcept override;
        PlayerComponent(PlayerComponent&&) noexcept = delete;
        PlayerComponent(PlayerComponent const&) noexcept = delete;
        PlayerComponent& operator=(PlayerComponent const&) noexcept = delete;
        PlayerComponent& operator=(PlayerComponent&&) noexcept = delete;

        void OnNotify(Event event, Subject const& caller) noexcept override;

        [[nodiscard]] uint32_t GetPoints() const noexcept{ return m_points; };
        void AddPoints(uint32_t points) noexcept;;

    private:
        uint32_t m_points{};
        MovementComponent& m_movementComponent;

        std::vector<DAE::Input::Binding> m_bindings{
            std::move({
                {SDL_SCANCODE_W, DAE::Input::InputMode::held},
                std::make_unique<Input::MoveCommand>(m_movementComponent, glm::vec2{ 0.f, -1.f })
            }),
            {
                {SDL_SCANCODE_A, DAE::Input::InputMode::held},
                std::make_unique<Input::MoveCommand>(m_movementComponent, glm::vec2{ -1.f, 0.f })
            },
            {
                {SDL_SCANCODE_S, DAE::Input::InputMode::held},
                std::make_unique<Input::MoveCommand>(m_movementComponent, glm::vec2{ 0.f, 1.f })
            },
            {
                {SDL_SCANCODE_D, DAE::Input::InputMode::held},
                std::make_unique<Input::MoveCommand>(m_movementComponent, glm::vec2{ 1.f, 0.f })
            },
            {
                {SDL_SCANCODE_P, DAE::Input::InputMode::released},
                std::make_unique<Input::PointCommand>(*this)
            }
        };

        Event m_onPointsIncreased{ MakeSDBMHash("OnPointsIncreased") };
        Event m_onCollected5Points{ MakeSDBMHash("OnCollected5Points") };
    };
}

#endif
