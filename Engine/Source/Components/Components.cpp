// Engine
#include "Components/Components.hpp"
#include "Rendering/Renderer.hpp"
#include "Scene/GameObject.hpp"
#include "Rendering/Font.hpp"
#include "Rendering/Sprite.hpp"
#include "Utils/Utils.hpp"
#include "Utils/Timer.hpp"
// Third-party
#include <format>
#include <SDL3_ttf/SDL_ttf.h>
#include <glm/glm.hpp>

/*******************************************
 * Render component
 *******************************************/

#pragma region RenderComponent
Engine::RenderComponent::RenderComponent(GameObject &owner,
    Sprite::View const& spriteView, Renderer::Layer const layer) noexcept
    : Component{owner}
    , m_spriteView{ spriteView }
{
    Renderer::GetInstance().RegisterFunction(m_renderFunction, layer);
    SetSpriteView({spriteView});
}

Engine::RenderComponent::~RenderComponent()
{
    Renderer::GetInstance().UnregisterFunction(m_renderFunction);
}

void Engine::RenderComponent::Render() const noexcept {
    if (not m_active) return;
    assert(m_spriteView.pSprite && "Texture is not set");
    auto const& topLeft{ m_owner.GetWorldLocation() };
    if (m_spriteView.srcRect.w > 0.f && m_spriteView.srcRect.h > 0.f)
    {
        Renderer::GetInstance().RenderTexture(
            *m_spriteView.pSprite,
            m_spriteView.srcRect,
            {
                topLeft.x, topLeft.y,
                m_spriteView.srcRect.w,
                m_spriteView.srcRect.h
            },
            m_degrees,
            m_flipMode
        );
    }
    else
    {
        Renderer::GetInstance().RenderTexture(
            *m_spriteView.pSprite,
            topLeft
        );
    }
}

void Engine::RenderComponent::SetSpriteView(Sprite::View const& spriteView) noexcept
{
    m_spriteView = spriteView;
}

void Engine::RenderComponent::SetSrcRect(SDL_FRect const& srcRect) noexcept
{
    m_spriteView.srcRect = srcRect;
}

void Engine::RenderComponent::SetSrcWidth(float const width) noexcept
{
    m_spriteView.srcRect.w = width;
}

glm::vec2 Engine::RenderComponent::GetSpriteViewDims() const noexcept
{
    auto const& srcRect{ m_spriteView.srcRect };
    return {srcRect.w, srcRect.h};
}
#pragma endregion RenderComponent

/*******************************************
 * Debug renderer
 *******************************************/

#pragma region DebugRenderer
Engine::DebugComponent::DebugComponent(GameObject& owner, Renderer& renderer) noexcept
    : Component{ owner }
    , m_renderer{ renderer }
{
    m_renderer.RegisterFunction(m_debugRenderFunction);
}

Engine::DebugComponent::~DebugComponent()
{
    m_renderer.UnregisterFunction(m_debugRenderFunction);
}
#pragma endregion DebugRenderer

/*******************************************
 * Text component
 *******************************************/

#pragma region TextComponent

Engine::TextComponent::TextComponent(GameObject& owner,
    std::string_view const text, Font* pFont, SDL_Color const& color) noexcept
    : Component(owner)
    , m_text{ text }, m_pFont{ pFont }, m_color{ color }
    , m_renderComponent{
        owner.AddComponent<RenderComponent>(
            Sprite::View{GetUpdatedTexture()}
        )
    }
{}

void Engine::TextComponent::SetFont(Font* pFont) {
    if (m_pFont == pFont) return;
    m_pFont = pFont;
    UpdateTexture();
}

void Engine::TextComponent::SetText(std::string_view const text) {
    // Re-rendering texture only if the text changed
    if (m_text == text) return;
    m_text = text;
    UpdateTexture();
}

bool AreColorsEqual(const SDL_Color& lhs, const SDL_Color& rhs) {
    return (lhs.r == rhs.r) && (lhs.g == rhs.g) && (lhs.b == rhs.b) && (lhs.a == rhs.a);
}

void Engine::TextComponent::SetColor(SDL_Color const &color)
{
    // Re-rendering texture only if the color changed
    if (AreColorsEqual(m_color, color)) return;
    m_color = color;
    UpdateTexture();
}

Engine::Sprite* Engine::TextComponent::GetUpdatedTexture()
{
    SDL_Surface* const pSurface{ TTF_RenderText_Blended(m_pFont->GetFont(), m_text.c_str(), m_text.length(), m_color) };

    // Transparency support
    Utils::Check(
        SDL_SetSurfaceBlendMode(pSurface, SDL_BLENDMODE_BLEND),
        "Failed to set surface blend mode"
    );

    if (!pSurface)
    {
        Utils::ThrowSDLError("Render text failed");
    }
    SDL_Texture* pSDLTexture{ SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), pSurface) };
    if (!pSDLTexture)
    {
        Utils::ThrowSDLError("Create text texture from surface failed");
    }
    SDL_DestroySurface(pSurface);
    m_pTexture = std::make_unique<Sprite>(pSDLTexture);
    return m_pTexture.get();
}

void Engine::TextComponent::UpdateTexture() {
    m_renderComponent.SetSpriteView(Sprite::View{GetUpdatedTexture()});
}
#pragma endregion TextComponent

/*******************************************
 * FPS component
 *******************************************/

#pragma region FPSComponent
Engine::FPSComponent::FPSComponent(GameObject &owner, Font* pFont, SDL_Color const& color) noexcept
    : Component(owner)
{
    owner.AddComponent<TextComponent>("FPS",  pFont, color);
}

void Engine::FPSComponent::Update() noexcept {
    Component::Update();
    // TODO: Make a check if the FPS is not the same as last time. Maybe we can omit creating a texture.
    m_owner.GetComponent<TextComponent>()->SetText(std::format("FPS: {:.0f}", Timer::GetInstance().GetFPS()));
}
#pragma endregion FPSComponent

/*******************************************
 * Orbit component
 *******************************************/

#pragma region OrbitComponent
Engine::OrbitComponent::OrbitComponent(GameObject& owner, float const radiansSec) noexcept
    : Component(owner)
    , m_radiansSec{ radiansSec }
{}

void Engine::OrbitComponent::Update() noexcept
{
    Component::Update();

    // 1. Getting the normalized distance vector and distance
    auto const distanceVector{ m_owner.GetWorldLocation() - m_owner.hierarchyElement.GetParentGameObject()->GetWorldLocation() };
    // NOTE: glm::vec2::length always returns 2 for some reason
    float const distance{ glm::sqrt(distanceVector.x * distanceVector.x + distanceVector.y * distanceVector.y) };
    glm::vec2 const distanceVectorNormalized{ glm::normalize(distanceVector) };
    assert(distanceVectorNormalized.length() > 0.f);
    // 2. Getting the angle of the distance vector
    float radians{ glm::atan(distanceVectorNormalized.y, distanceVectorNormalized.x) };
    // 3. Adding angular velocity to the angle
    radians += m_radiansSec * Timer::GetInstance().GetDeltaSec();
    // 4. Calculating the new distance vector and adding the distance vector to the center
    m_owner.SetLocalPosition(glm::vec2(glm::cos(radians), std::sin(radians)) * distance);
}
#pragma endregion OrbitComponent
