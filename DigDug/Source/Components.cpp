#include "Components.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "GameObject.h"
#include "Font.h"
#include "Texture2D.h"
#include "Utils.h"
#include "Timer.h"
#include <SDL3_ttf/SDL_ttf.h>

/*******************************************
 * Render component
 *******************************************/
DAE::Components::RenderComponent::RenderComponent(GameObject &owner) noexcept
    : Components(owner) {
    m_pTransformComponent = owner.AddComponent<TransformComponent>(owner);
    Renderer::GetInstance().RegisterComponent(this);
}

DAE::Components::RenderComponent::~RenderComponent()
{
    Renderer::GetInstance().UnregisterComponent(this);
}

void DAE::Components::RenderComponent::Render() const {
    assert(m_pTexture && "Texture is not set");
    Renderer::GetInstance().RenderTexture(
        *m_pTexture,
        m_pTransformComponent->GetTransform().GetLocation());
}

void DAE::Components::RenderComponent::SetTexture(std::string_view const filename) {
    m_pTexture = ResourceManager::GetInstance().LoadTexture(filename);
}

void DAE::Components::RenderComponent::SetTexture(SDL_Texture* pSDLTexture) {
    m_pTexture = std::make_shared<Texture2D>(pSDLTexture);
}

/*******************************************
 * Text component
 *******************************************/
DAE::Components::TextComponent::TextComponent(GameObject& owner, std::string_view const text, std::shared_ptr<Font> const &pFont, SDL_Color const& color) noexcept
    : Components(owner)
    , m_text{ text }, m_pFont{ pFont }, m_color{ color }
{
    m_pRenderComponent = owner.AddComponent<RenderComponent>(owner);
    UpdateTexture();
}

void DAE::Components::TextComponent::SetFont(std::shared_ptr<Font> const &pFont) {
    m_pFont = pFont;
    UpdateTexture();
}

void DAE::Components::TextComponent::SetText(std::string_view const text) {
    m_text = text;
    UpdateTexture();
}

void DAE::Components::TextComponent::SetColor(SDL_Color const &color) {
    m_color = color;
    UpdateTexture();
}

void DAE::Components::TextComponent::UpdateTexture() const {
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
    m_pRenderComponent->SetTexture(pSDLTexture);
}

/*******************************************
 * FPS component
 *******************************************/
DAE::Components::FPSComponent::FPSComponent(GameObject &owner, std::shared_ptr<Font> const& pFont, SDL_Color const& color) noexcept
    : Components(owner)
{
    owner.AddComponent<TextComponent>(owner, "FPS", pFont, color);
}

void DAE::Components::FPSComponent::Update() noexcept {
    Components::Update();
    // TODO: Make a check if the FPS is not the same as last time. Maybe we can omit creating a texture.
    m_owner.GetComponent<TextComponent>()->SetText(std::format("FPS: {:.0f}", Timer::GetInstance().GetFPS()));
}
