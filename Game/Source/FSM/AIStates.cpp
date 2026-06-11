// Game
#include "FSM/AIStates.hpp"
#include "Utils.hpp"
#include "Levels/Level.hpp"
// Engine
#include "Engine/Components/AnimationComponent.hpp"
// Third-party
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/glm.hpp"
#include "glm/gtx/norm.hpp"
// Standard
#include <queue>
#include <set>
#include <print>

#pragma region StateBase
Game::AI::AIStateBase::AIStateBase(Dependencies const& dependencies)
    : StateBase{ dependencies } {}

Game::AI::AIStateBase::Path Game::AI::AIStateBase::TryFindingPathToPlayer() noexcept// BFS
{
    Cell startCell{  m_dependencies.level.GetGrid().GetCellFromPoint(m_dependencies.owner.GetWorldLocation())  };
    Cell const targetCell{ m_dependencies.level.GetPlayerCell()};
    // Pending
    std::queue<Cell> pending;
    pending.emplace(startCell);
    // Visited
    std::set<Cell, Engine::Utils::StrictWeakComparor_i32vec2> visited;
    visited.insert(startCell);
    // Parents
    CellMap parents;

    while (not pending.empty())
    {
        Cell const currentCell{ pending.front() };
        pending.pop();
        if (currentCell == targetCell) return ReconstructPath(parents, startCell, targetCell);
        for (Cell neighbor : GetNeighbors(currentCell))
        {
            if (visited.contains(neighbor)) continue;
            visited.insert(neighbor);
            parents[neighbor] = currentCell;
            pending.emplace(neighbor);
        }
    }

    return {};
}

Game::AI::AIStateBase::Path Game::AI::AIStateBase::ReconstructPath(CellMap const& parents, Cell const startCell, Cell const endCell) noexcept
{
    Cell currentCell{ endCell };
    Path path;

    while (currentCell != startCell)
    {
        path.push_back(currentCell);
        currentCell = parents.at(currentCell);
    }

    path.push_back(startCell);

    std::ranges::reverse(path);

    return path;
}

std::vector<Game::AI::AIStateBase::Cell> Game::AI::AIStateBase::GetNeighbors(Cell const cell) const noexcept
{
    std::vector<Cell> cells;
    Grid const& grid{ m_dependencies.level.GetGrid() };

    std::array<Cell, 4> constexpr directions = {{
        {-1, 0},  // left
        { 1, 0},  // right
        { 0, 1},  // bottom
        { 0,-1}   // top
    }};

    for (auto const& dir : directions)
    {
        // Checking if the cell is within grid
        if (Cell const neighbor{ cell.x + dir.x, cell.y + dir.y };
            neighbor.x >= 0 && neighbor.y >= 0
            && neighbor.x < m_dependencies.level.GetGrid().GetDimsInCells().x
            && neighbor.y < m_dependencies.level.GetGrid().GetDimsInCells().y)
        {
            if (not grid.IsGround(neighbor)) cells.emplace_back(neighbor);
        }
    }

    return cells;
}

#pragma endregion StateBase

#pragma region Wander
template<typename Direction>
Game::AI::Wander<Direction>::Wander(Dependencies const& dependencies)
    : AIStateBase{ dependencies }
    , m_moveCommand1{ dependencies.movementComponent,
        std::is_same_v<Direction, Horizontal> ? glm::vec2{-1.f, 0.f} : glm::vec2{0.f, 1.f} }
    , m_moveCommand2{ dependencies.movementComponent,
        std::is_same_v<Direction, Horizontal> ? glm::vec2{1.f, 0.f} : glm::vec2{0.f, -1.f} }
    , m_pCurrentCommand{ &m_moveCommand2 }  // Start moving positive direction
{}

template<typename Direction>
Game::StateType Game::AI::Wander<Direction>::Update() noexcept
{
    if (m_playerReachable) return typeid(Chase);

    glm::vec2 const currentLocation{ m_dependencies.owner.GetWorldLocation() };

    // Flipping direction when facing dead end
    if (Engine::Utils::NearlyEqual(currentLocation, m_prevLocation))
        FlipDirection();

    m_prevLocation = currentLocation;
    m_pCurrentCommand->Execute();

    return std::nullopt;
}

template<typename Direction>
void Game::AI::Wander<Direction>::OnEnter() noexcept
{
    // Changing animation
    m_dependencies.animationComponent.ChangeAnimation(
        SDL_FRect{0.f, 0.f,
            static_cast<float>(tileSideLength),
            static_cast<float>(tileSideLength)},
        2
    );

    // Subscribing to the grid changing event
    m_dependencies.level.GetGrid().BindObserver(*this);
}

template <typename Direction>
void Game::AI::Wander<Direction>::OnExit() noexcept
{
    // Unsubscribing from the grid changing event
    m_dependencies.level.GetGrid().UnbindObserver(*this);
}

template<typename Direction>
void Game::AI::Wander<Direction>::FlipDirection() noexcept
{
    if (m_pCurrentCommand == &m_moveCommand1)
        m_pCurrentCommand = &m_moveCommand2;
    else
        m_pCurrentCommand = &m_moveCommand1;
}

template <typename Direction>
void Game::AI::Wander<Direction>::OnNotify(Engine::Event const event, Engine::Subject const& caller) noexcept
{
    switch (event.id)
    {
    case std::to_underlying(EventType::OnGridChanged):
    {
        // Chasing if player is reachable
       if (Path const path{ TryFindingPathToPlayer() };
            not path.empty())
        {
            m_playerReachable = true;
        }
    }
    default:;
    }
}

template class Game::AI::Wander<Game::AI::Horizontal>;
template class Game::AI::Wander<Game::AI::Vertical>;
#pragma endregion

#pragma region Chase
Game::AI::Chase::Chase(Dependencies const& dependencies)
    : AIStateBase{ dependencies }{}

Game::StateType Game::AI::Chase::Update() noexcept
{
    if (m_path.empty())
    {
        m_path = TryFindingPathToPlayer();
        m_dependencies.movementComponent.Disable();
        return std::nullopt;
    }
    // Moving towards the current cell
    Grid const& grid{ m_dependencies.level.GetGrid() };
    // Switching the target cell if we've arrived
    Cell const currentTargetCell{ m_path.at(m_currentTargetIdx) };
    if (glm::vec2 const npcToTarget{ grid.GetCellCenter(currentTargetCell) - grid.GetCellCenter(grid.GetCellFromPoint(m_dependencies.owner.GetWorldLocation() + glm::vec2{1, 1})) };
        Engine::Utils::NearlyZero(glm::length2(npcToTarget)))
    {
        // Last cell -> generating a new path
        if (m_currentTargetIdx == m_path.size() - 1)
        {
            m_path = TryFindingPathToPlayer();
            m_currentTargetIdx = 0;
        }
        else// Cells left -> moving to the next one
        {
            ++m_currentTargetIdx;
        }
    }
    else
    {
        m_dependencies.movementComponent.Enable();
        glm::vec2 const normDir{ normalize(npcToTarget) };
        std::println("Norm dir: {}, {}", normDir.x, normDir.y);
        m_dependencies.movementComponent.AddDirection(
            normDir
        );
    }

    return std::nullopt;
}

void Game::AI::Chase::OnEnter() noexcept
{
    m_path = TryFindingPathToPlayer();
    Engine::Renderer::GetInstance().RegisterFunction(m_debugRenderFunction);
}

void Game::AI::Chase::OnExit() noexcept
{
    m_currentTargetIdx = 0;
    Engine::Renderer::GetInstance().UnregisterFunction(m_debugRenderFunction);
}

void Game::AI::Chase::DebugRender() const noexcept
{
    if (m_path.empty()) return;
    Engine::Renderer::GetInstance().RenderSquare(
        Engine::Utils::Square{m_dependencies.level.GetGrid().GetCellTopLeft(m_path.at(m_currentTargetIdx)),
        tileSideLength},
        SDL_FColor{0, 255, 0, 255}
    );
}
#pragma endregion Chase
