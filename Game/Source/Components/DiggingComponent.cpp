// Game
#include "Components/DiggingComponent.hpp"
// Engine
#include "Constants.hpp"
#include "Engine/Rendering/Renderer.hpp"
#include "Engine/Scene/GameObject.hpp"

Game::DiggingComponent::DiggingComponent(Engine::GameObject& owner) noexcept
    : Component(owner)
    , m_pSDLRenderer{ Engine::Renderer::GetInstance().GetSDLRenderer() }
{
    auto& renderer{ Engine::Renderer::GetInstance() };

    // Registering the render callback to the renderer
    renderer.RegisterFunction(m_renderFunction);

    // Creating mask texture
    m_maskTexture = SDL_CreateTexture(m_pSDLRenderer, SDL_PIXELFORMAT_RGBA8888,
                                       SDL_TEXTUREACCESS_TARGET,
                                       static_cast<int32_t>(windowData.logicalDims.x),
                                       static_cast<int32_t>(windowData.logicalDims.y));

    // Initializing the mask with all white
    SDL_SetRenderTarget(m_pSDLRenderer, m_maskTexture);
    SDL_SetRenderDrawColor(m_pSDLRenderer, 255, 255, 255, 255);
    SDL_RenderClear(m_pSDLRenderer);
    SDL_SetRenderTarget(m_pSDLRenderer, nullptr);
}

Game::DiggingComponent::~DiggingComponent() noexcept
{
    Engine::Renderer::GetInstance().UnregisterFunction(m_renderFunction);

    SDL_DestroyTexture(m_maskTexture);// TODO: Use RAII wrapper for texture
}

void Game::DiggingComponent::Render() const noexcept
{
    ApplyMask();
    // Digging a circle
    SDL_SetRenderTarget(m_pSDLRenderer, m_maskTexture);
    static auto constexpr offset{ 0.5f * glm::vec2{tileSideLength, tileSideLength} };
    static float constexpr halfTileSideLength{ 0.5f * tileSideLength };
    Engine::Renderer::GetInstance().RenderFilledCircle(m_owner.GetWorldLocation() + offset, halfTileSideLength);
    SDL_SetRenderTarget(m_pSDLRenderer, nullptr);
}

void Game::DiggingComponent::ApplyMask() const noexcept
{
    SDL_SetTextureBlendMode(m_maskTexture, SDL_BLENDMODE_MOD);
    SDL_RenderTexture(m_pSDLRenderer, m_maskTexture, nullptr, nullptr);
    SDL_SetRenderTarget(m_pSDLRenderer, nullptr);
}
