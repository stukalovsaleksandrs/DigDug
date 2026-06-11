#ifndef GAME_AI_FSM
#define GAME_AI_FSM

// Game
#include "FSM.hpp"
// Engine
#include "Engine/Commands.hpp"
#include "Engine/Core/Observer.hpp"
#include "Engine/Utils/Utils.hpp"

namespace Game::AI
{
    class AIStateBase : public StateBase
    {
    public:
        using Cell = glm::i32vec2;
        using CellMap = std::unordered_map<Cell, Cell, Engine::Utils::Hash_i32vec2>;
        using Path = std::vector<glm::i32vec2>;

        explicit AIStateBase(Dependencies const& dependencies);
        [[nodiscard]] Path TryFindingPathToPlayer() noexcept;

    private:
        [[nodiscard]] Path ReconstructPath(CellMap const& parents, Cell startCell, Cell endCell) noexcept;
        [[nodiscard]] std::vector<Cell> GetNeighbors(Cell) const noexcept;

    };

#pragma region Wander
    // Direction tags
    struct Horizontal {};
    struct Vertical {};

    template<typename>
    class Wander final : public AIStateBase, public Engine::Observer
    {
    public:
        explicit Wander(Dependencies const& dependencies);
        StateType Update() noexcept override;
        void OnEnter() noexcept override;
        void OnExit() noexcept override;

    private:
        Engine::MoveCommand m_moveCommand1;
        Engine::MoveCommand m_moveCommand2;
        Engine::MoveCommand* m_pCurrentCommand;
        glm::vec2 m_prevLocation{};
        bool m_playerReachable{};

        void FlipDirection() noexcept;
        void OnNotify(Engine::Event event, Engine::Subject const& caller) noexcept override;
    };

    using WanderHorizontally = Wander<Horizontal>;
    using WanderVertically = Wander<Vertical>;
#pragma endregion Wander

    class Chase final : public AIStateBase
    {
    public:
        explicit Chase(Dependencies const& dependencies);
        StateType Update() noexcept override;
        void OnEnter() noexcept override;
        void OnExit() noexcept override;
        void DebugRender() const noexcept;

    private:
        Path m_path;
        uint32_t m_currentTargetIdx{};

        std::function<void()> m_debugRenderFunction{
            [this]
            {
                DebugRender();
            }
        };
    };

}

#endif// GAME_AI_FSM
