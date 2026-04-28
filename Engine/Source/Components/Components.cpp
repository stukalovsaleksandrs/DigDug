#include "Components/Components.h"
#include "Core/ResourceManager.h"
#include "Rendering/Renderer.h"
#include "Scene/GameObject.h"
#include "Rendering/Font.h"
#include "Rendering/Texture2D.h"
#include "Utils/Utils.h"
#include "Utils/Timer.h"
#include <SDL3_ttf/SDL_ttf.h>
#include <glm/glm.hpp>

/*******************************************
 * Render component
 *******************************************/

Engine::RenderComponent::RenderComponent(GameObject &owner) noexcept
    : Component(owner)
{
    Renderer::GetInstance().RegisterFunction(m_renderFunction);
}

Engine::RenderComponent::~RenderComponent()
{
    Renderer::GetInstance().UnregisterFunction(m_renderFunction);
}

void Engine::RenderComponent::Render() const {
    assert(m_pTexture && "Texture is not set");
    Renderer::GetInstance().RenderTexture(
        *m_pTexture,
        m_owner.GetWorldPosition());
}

void Engine::RenderComponent::SetTexture(std::string_view const filename) {
    m_pTexture = ResourceManager::GetInstance().LoadTexture(filename);
}

void Engine::RenderComponent::SetTexture(SDL_Texture* pSDLTexture) {
    m_pTexture = std::make_shared<Texture2D>(pSDLTexture);
}

glm::vec2 Engine::RenderComponent::GetTextureDims() const noexcept
{
    return m_pTexture->GetDims();
}

/*******************************************
 * Debug renderer
 *******************************************/

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

/*******************************************
 * Text component
 *******************************************/

Engine::TextComponent::TextComponent(GameObject& owner,
    std::string_view const text, std::shared_ptr<Font> const &pFont, SDL_Color const& color) noexcept
    : Component(owner)
    , m_text{ text }, m_pFont{ pFont }, m_color{ color }
    , m_renderComponent{owner.AddComponent<RenderComponent>()}
{
    UpdateTexture();
}

void Engine::TextComponent::SetFont(std::shared_ptr<Font> const &pFont) {
    // NOTE: I am not aware of any way to compare whether fonts are the same, but
    // I can compare whether the 2 pointers point to the same object
    if (m_pFont.get() == pFont.get()) return;
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

void Engine::TextComponent::UpdateTexture() const {
    SDL_Surface* const pSurface{ TTF_RenderText_Blended(m_pFont->GetFont(), m_text.c_str(), m_text.length(), m_color) };
    if (!pSurface)
    {
        Utils::ThrowSDLError("Render text failed");
    }
    auto const pSDLTexture{ SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), pSurface) };
    if (!pSDLTexture)
    {
        Utils::ThrowSDLError("Create text texture from surface failed");
    }
    SDL_DestroySurface(pSurface);
    m_renderComponent.SetTexture(pSDLTexture);
}

/*******************************************
 * FPS component
 *******************************************/

Engine::FPSComponent::FPSComponent(GameObject &owner, std::shared_ptr<Font> const& pFont, SDL_Color const& color) noexcept
    : Component(owner)
{
    owner.AddComponent<TextComponent>("FPS",  pFont, color);
}

void Engine::FPSComponent::Update() noexcept {
    Component::Update();
    // TODO: Make a check if the FPS is not the same as last time. Maybe we can omit creating a texture.
    m_owner.GetComponent<TextComponent>()->SetText(std::format("FPS: {:.0f}", Timer::GetInstance().GetFPS()));
}

/*******************************************
 * Orbit component
 *******************************************/

Engine::OrbitComponent::OrbitComponent(GameObject& owner, float const radiansSec) noexcept
    : Component(owner)
    , m_radiansSec{ radiansSec }
{}

void Engine::OrbitComponent::Update() noexcept
{
    Component::Update();

    // 1. Getting the normalized distance vector and distance
    auto const distanceVector{ m_owner.GetWorldPosition() - m_owner.hierarchyElement.GetParentGameObject()->GetWorldPosition() };
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
