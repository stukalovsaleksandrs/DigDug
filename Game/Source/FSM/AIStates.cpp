// #define ENABLE_DEBUG_RENDERING
#define GLM_ENABLE_EXPERIMENTAL

// Game
#include "FSM/AIStates.hpp"
#include "Utils.hpp"
#include "Levels/Level.hpp"
// Engine
#include "Engine/Components/AnimationComponent.hpp"
// Third-party
#include "glm/glm.hpp"
#include "glm/gtx/norm.hpp"
// Standard
#include <queue>
#include <set>
#include <print>

#include "Engine/Utils/Timer.hpp"

#pragma region StateBase
Game::AI::AIStateBase::AIStateBase(Dependencies const& dependencies)
    : m_dependencies{ dependencies }
    , m_movementComponent{ *dependencies.owner.GetComponent<Engine::MovementComponent>() }
    , m_animationComponent{ *dependencies.owner.GetComponent<Engine::AnimationComponent>() }
{}

Game::AI::AIStateBase::Path Game::AI::AIStateBase::TryFindingPathToPlayer() const noexcept// BFS
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

Game::StateType Game::AI::AIStateBase::ProcessGameAction(EventType const type) noexcept
{
    switch (type)
    {
    case EventType::OnCaught:
        return typeid(Pumped);
    default:
        break;
    }
    return StateBase::ProcessGameAction(type);
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
    , m_moveCommand1{ m_movementComponent,
        std::is_same_v<Direction, Horizontal> ? glm::vec2{-1.f, 0.f} : glm::vec2{0.f, 1.f} }
    , m_moveCommand2{ m_movementComponent,
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
    m_animationComponent.ChangeSource(
        SDL_FRect{0.f, 0.f,
            ftileSideLengthPx,
            ftileSideLengthPx},
        2, 0.2f
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
        m_movementComponent.SetActive(false);
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
        m_movementComponent.SetActive(true);
        glm::vec2 const normDir{ normalize(npcToTarget) };
        m_movementComponent.AddDirection(
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
#ifdef ENABLE_DEBUG_RENDERING
    if (m_path.empty()) return;
    Engine::Renderer::GetInstance().RenderSquare(
        Engine::Utils::Square{m_dependencies.level.GetGrid().GetCellTopLeft(m_path.at(m_currentTargetIdx)),
        i32tileSideLengthPx},
        SDL_FColor{0, 255, 0, 255}
    );
#endif// ENABLE_DEBUG_RENDERING
}
#pragma endregion Chase

#pragma region Pumped
Game::AI::Pumped::Pumped(Dependencies const& dependencies)
    : AIStateBase{dependencies}{}
float firstInit{true};
void Game::AI::Pumped::OnEnter() noexcept
{
    AIStateBase::OnEnter();
    Engine::Sprite::View const view{
        m_dependencies.level.sharedResources.pPookaPumpedSprite.get(),
        SDL_FRect{ 0.f, 0.f, 32.f, 32.f }
    };
    m_animationComponent.ChangeAnimation({
        .firstSpriteView = view,
        .frameCount = m_frameCount,
        .secPerFrame = m_secPerFrame
    });
    auto& owner{ m_animationComponent.GetOwner() };
    auto& renderComponent{ *owner.GetComponent<Engine::RenderComponent>() };
    renderComponent.dstDims = glm::vec2{32.f, 32.f};
    renderComponent.SetSpriteView(view);
    auto const ownerWorldLocation{ owner.GetWorldLocation() };
    owner.SetLocalPosition(glm::vec2{ownerWorldLocation.x - ftileSideLengthPx, ownerWorldLocation.y - 0.5f * ftileSideLengthPx});
}

Game::StateType Game::AI::Pumped::Update() noexcept
{
    m_currentSec += Engine::Timer::GetInstance().GetDeltaSec();
    if (m_currentSec >= m_secPerFrame * m_frameCount)
    {
        // Notifying the player character
        NotifyObservers(EventType::OnEnemyDied);

        // Deleting the enemy
        m_animationComponent.SetActive(false);
        m_dependencies.owner.GetComponent<Engine::RenderComponent>()->SetActive(false);
        m_dependencies.owner.MarkForDeletion();
    }
    return StateBase::Update();
}

#pragma endregion Pumped
