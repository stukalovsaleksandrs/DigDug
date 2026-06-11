// Game
#include "Levels/Level.hpp"
#include "Levels/LevelManager.hpp"
#include "Components/LivesDisplayComponent.hpp"
#include "Components/PointDisplayComponent.hpp"
#include "Components/AIComponent.hpp"
#include "Components/PlayerComponent.hpp"
#include "Components/LivesComponent.hpp"
#include "Components/PumpComponent.hpp"
// Engine
#include "Engine/Rendering/Renderer.hpp"
#include "Engine/Components/MovementComponent.hpp"
#include "Engine/Components/AnimationComponent.hpp"
// Standard
#include <format>
#include <fstream>
#include <print>

Game::Level::Level(std::string_view const path, Resources const& sharedResources) noexcept
    : m_sharedResources{ sharedResources }
    , m_maskTexture{ SDL_Color{255, 255, 255, 255}, windowData.logicalDims, SDL_BLENDMODE_MOD}
    , m_charToParsingFunc{
        { '1', [this](glm::i32vec2 const cell)
        {
            if (m_playerSpawnCell != glm::u32vec2{})
                throw std::runtime_error{ std::format("A file has multiple character spawn points") };

            m_playerSpawnCell = cell;
            m_grid.SetAir(cell);
        }},
        { '2', [this](glm::i32vec2 const cell)
        {
            m_pookaSpawnCells.emplace_back(cell);
            m_grid.SetAir(cell);
        }},
        {'3', [this](glm::i32vec2 const cell)
        {
            m_flygarSpawnCells.emplace_back(cell);
            m_grid.SetAir(cell);
        }},
        {'*', [this](glm::i32vec2 const cell)
        {
            m_rockSpawnCells.emplace_back(cell);
        }},
        {'.', [](glm::i32vec2 const)
        {
            // grid is all ground by default
            // I do still need a callback, otherwise there'll be an exception
        }},
        {'-', [this](glm::i32vec2 const cell)
        {
            m_grid.SetAir(cell);
        }}
    }
{
    ParseFile(path);

    // Ground
    {
        auto& ground{m_scene.CreateGameObject({})};
        ground.AddComponent<Engine::RenderComponent>(Engine::Sprite::View{m_sharedResources.pGroundSprite.get()}, Engine::Renderer::Layer::background);
    }

    // Registering the tunnel digging render callback to the renderer
    Engine::Renderer::GetInstance().RegisterFunction(m_renderTunnelsFunction, Engine::Renderer::Layer::background);

    // Sky
    {
        auto& sky{m_scene.CreateGameObject({})};
        sky.AddComponent<Engine::RenderComponent>(Engine::Sprite::View{m_sharedResources.pSkySprite.get()}, Engine::Renderer::Layer::middleground);
    }


    // Spawning
    SpawnPlayer();
    SpawnPookas();
    SpawnPump();
}

Game::Level::~Level() noexcept
{
    Engine::Renderer::GetInstance().UnregisterFunction(m_renderTunnelsFunction);
}

void Game::Level::DigCircle(glm::vec2 const centerPx) const noexcept
{
    static float constexpr halfTileSideLength{ 0.5f * tileSideLength };
    m_maskTexture.MaskCircle({centerPx, halfTileSideLength});
}

bool Game::Level::TryDigging(glm::vec2 const cellCenterPx) noexcept
{
    if (m_grid.TryDigging(cellCenterPx))
    {
        DigCircle(cellCenterPx);
        return true;
    }
    return false;
}

glm::u32vec2 Game::Level::GetPlayerCell() const noexcept
{
    return m_grid.GetCellFromPoint(m_pPlayer->GetWorldLocation() + glm::vec2{1.f, 1.f});
}

void Game::Level::Update() noexcept
{
    m_scene.Update();
}

void Game::Level::EnablePump() const noexcept
{
    m_pPump->SetActive(true);
}

void Game::Level::SpawnPump() noexcept
{
    if (m_pPumpComponent) return;// No pump spamming
    m_pPump = &m_scene.CreateGameObject(*m_pPlayer, glm::vec2{0.5f * tileSideLength, 0.f});

    m_pPump->AddComponent<Engine::RenderComponent>(
        Engine::Sprite::View{ m_sharedResources.pPumpSprite.get() }
    );

    m_pPumpComponent = &m_pPump->AddComponent<PumpComponent>();

    m_pPump->SetActive(false);
}

Engine::MovementComponent::CanMovePred Game::Level::GetCanMovePred() const noexcept
{
    return [this](glm::vec2 const topLeft)
    {
        auto const topLeftPointCell{ m_grid.GetCellFromPoint(topLeft + glm::vec2{1.f, 1.f}) },
            bottomRightPointCell{ m_grid.GetCellFromPoint(topLeft + glm::vec2{tileSideLength - 1.f, tileSideLength - 1.f}) };
        return not (m_grid.IsGround(topLeftPointCell) || m_grid.IsGround(bottomRightPointCell));
    };
}

[[nodiscard]] std::string_view GetExtension(std::string_view const path)
{
    return path.substr(path.find_last_of('.') + 1);
}

void ValidateExtension(std::string_view const path, std::string_view const extension)
{
    if (!path.ends_with(extension)) {
        throw std::runtime_error(
            std::format("Level file extension is incorrect: got {}, want {}", GetExtension(path), extension)
        );
    }
}

void Game::Level::ParseFile(std::string_view const path) const
{
    // Validating file extension
    ValidateExtension(path, ".txt");

    // Open the file, checking if exists
    std::ifstream file{ path.data() };
    if (not file.is_open())
        throw std::runtime_error{ std::format("Failed to open level file: {}", path) };

    std::string line{};
    uint32_t row{};
    while (std::getline(file, line))
    {
        for (uint32_t const col : std::ranges::views::iota(0u, line.size()))
        {
            glm::u32vec2 const cell{ col, row };
            ParsePlayer(line, cell);
        }
        ++row;
    }

    MaskInitialTunnels();
}

void Game::Level::ParsePlayer(std::string_view const line, glm::u32vec2 const cell) const
{
    char const c{ line.at(cell.x) };
    m_charToParsingFunc.at(c)(cell);
}

void Game::Level::DigSquare(glm::vec2 const topLeftPx, EU::Square::Corners const corners) const noexcept
{
    m_maskTexture.MaskSquare({
        topLeftPx,
        static_cast<float>(tileSideLength),
        corners
    });
}

void Game::Level::MaskInitialTunnels() const noexcept
{
    glm::i32vec2 const dims{
        static_cast<int32_t>(windowData.logicalDims.x) / tileSideLength,
        static_cast<int32_t>(windowData.logicalDims.y) / tileSideLength
    };

    for (int32_t row{}; row < dims.y; ++row)
    {
        for (int32_t col{}; col < dims.x; ++col)
        {
            glm::i32vec2 const cell{ col, row };
            if (m_grid.IsGround(cell)) continue;

            // Check the four cardinal neighbors
            bool const airLeft  { !m_grid.IsGround({ col - 1, row }) };
            bool const airRight { !m_grid.IsGround({ col + 1, row }) };
            bool const airUp    { !m_grid.IsGround({ col, row - 1 }) };
            bool const airDown  { !m_grid.IsGround({ col, row + 1 }) };

            // A corner is sharp if ANY adjacent neighbor in that corner's directions is air.
            // e.g. top-left corner is sharp if the cell to the left OR above is air.
            EU::Square::Corners const corners
            {
                .topLeft     = !airLeft  && !airUp,
                .topRight    = !airRight && !airUp,
                .bottomLeft  = !airLeft  && !airDown,
                .bottomRight = !airRight && !airDown,
            };

            DigSquare(m_grid.GetCellTopLeft(cell), corners);
        }
    }
}

void Game::Level::SpawnPlayer() noexcept
{
    m_pPlayer = &m_scene.CreateGameObject({1.f, tileSideLength + 1});

    // Movement component
    m_pPlayer->AddComponent<Engine::MovementComponent>(
        Engine::MovementComponent::Dependencies{windowData, tileSideLength},
        tileSideLength,
        45.f
    );

    // Animation component
    m_pPlayer->AddComponent<Engine::AnimationComponent>(Engine::AnimationComponent::Data{
        .firstSpriteView = Engine::Sprite::View{m_sharedResources.pTaizoHoriSprite.get(),
            SDL_FRect{0.f, 0.f,
            static_cast<float>(tileSideLength),
            static_cast<float>(tileSideLength)}
            },
        .frameCount = 2,
        .secPerFrame = 0.2f
    });

    // Player component(must be added after animation component)
    auto& playerComponent{m_pPlayer->AddComponent<PlayerComponent>(PlayerComponent::Dependencies{*this})};

    // Render component
    auto& characterRenderComponent{m_pPlayer->AddComponent<Engine::RenderComponent>(
        Engine::Sprite::View{m_sharedResources.pTaizoHoriSprite.get()}
    )};
    characterRenderComponent.SetSpriteView({m_sharedResources.pTaizoHoriSprite.get(), SDL_FRect{0.f, 0.f,
        static_cast<float>(tileSideLength), static_cast<float>(tileSideLength)}});

    // Lives component
    auto& livesComponent{m_pPlayer->AddComponent<LivesComponent>(2)};
    livesComponent.BindObserver(playerComponent);

    // Lives display
    {
        auto& livesDisplay{m_scene.CreateGameObject(glm::vec2{10.f, static_cast<float>(windowData.physicalDims.y) - 10.f})};
        livesDisplay.AddComponent<Engine::TextComponent>(" ", m_sharedResources.pFont.get()); // NOTE: Text must not be empty
        auto& livesDisplayComponent{livesDisplay.AddComponent<LivesDisplayComponent>(livesComponent)};
        livesComponent.BindObserver(livesDisplayComponent);
    }

    // Point display
    {
        auto& pointDisplay{m_scene.CreateGameObject(glm::vec2{10.f, static_cast<float>(windowData.physicalDims.y) - 20.f})};
        pointDisplay.AddComponent<Engine::TextComponent>("Points ", m_sharedResources.pFont.get());
        auto& pointDisplayComponent{pointDisplay.AddComponent<PointDisplayComponent>(playerComponent)};
        playerComponent.BindObserver(pointDisplayComponent);
    }
}

void Game::Level::SpawnPookas() noexcept
{
    for (glm::i32vec2 const cell: m_pookaSpawnCells)
    {
        SpawnPooka(m_grid.GetCellTopLeft(cell));
    }
}

void Game::Level::SpawnPooka(glm::vec2 const topLeft) noexcept
{
    auto& pooka{m_scene.CreateGameObject(topLeft)};

    // Movement component
    pooka.AddComponent<Engine::MovementComponent>(
        Engine::MovementComponent::Dependencies{windowData, tileSideLength},
        tileSideLength,
        55.f,
        GetCanMovePred()
    );

    // Animation component
    pooka.AddComponent<Engine::AnimationComponent>(Engine::AnimationComponent::Data{
        .firstSpriteView = Engine::Sprite::View{m_sharedResources.pPookaSprite.get(),
            SDL_FRect{0.f, 0.f,
            static_cast<float>(tileSideLength),
            static_cast<float>(tileSideLength)}
            },
        .frameCount = 2,
        .secPerFrame = 0.3f
    });

    // Render component
    auto& renderComponent{pooka.AddComponent<Engine::RenderComponent>(
        Engine::Sprite::View{m_sharedResources.pPookaSprite.get()}
    )};
    renderComponent.SetSpriteView({m_sharedResources.pPookaSprite.get(), SDL_FRect{0.f, 0.f,
        static_cast<float>(tileSideLength), static_cast<float>(tileSideLength)}});

    // AI component
    pooka.AddComponent<AIComponent>(PawnComponent::Dependencies{*this});
}
