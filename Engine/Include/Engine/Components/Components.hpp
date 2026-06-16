#ifndef ENGINE_COMPONENTS
#define ENGINE_COMPONENTS

// Engine
#include "Engine/Components/ComponentBase.hpp"
#include "Engine/Rendering/Renderer.hpp"
#include "Engine/Rendering/Sprite.hpp"
// Third-party
#include <SDL3/SDL_pixels.h>
#include "glm/ext/scalar_constants.hpp"
#include "glm/vec2.hpp"
// Standard
#include <memory>
#include <string_view>
#include <string>
#include <functional>

namespace Engine
{
    class GameObject;
    class Sprite;
    class Font;

#pragma region RenderComponent
    class RenderComponent final : public Component {
        Sprite::View m_spriteView;

    public:
        struct Settings final{ SDL_FlipMode flipMode; float degrees; };
        glm::vec2 dstDims{};

        explicit RenderComponent(GameObject& owner, Sprite::View const&, Renderer::Layer layer = Renderer::Layer::foreground, uint32_t instanceCount = 1) noexcept;
        ~RenderComponent() override;
        RenderComponent(RenderComponent const&) = delete;
        RenderComponent(RenderComponent&&) = delete;
        RenderComponent& operator= (RenderComponent const&) = delete;
        RenderComponent& operator= (RenderComponent&&) = delete;

        void Render() const noexcept;
        void SetSpriteView(Sprite::View const&) noexcept;
        void SetSrcRect(SDL_FRect const&) noexcept;
        void SetSrcWidth(float width) noexcept;
        [[nodiscard]] glm::vec2 GetSpriteViewDims() const noexcept;
        [[nodiscard]] glm::vec2 GetSpriteDims() const noexcept{ return m_spriteView.pSprite->GetDims(); };
        void SetFlipMode(SDL_FlipMode const flipMode){ m_flipMode = flipMode; }
        void SetRotation(float const degrees){ m_degrees = degrees; }

        [[nodiscard]] Settings GetSettings() const noexcept{ return {m_flipMode, m_degrees}; }
        void SetSettings(Settings const& settings) noexcept{ m_flipMode = settings.flipMode; m_degrees = settings.degrees; }

        void SetInstanceCount(uint32_t const count) noexcept{ m_instanceCount = count; };

    private:
        std::function<void()> m_renderFunction{ [this]{this->Render();} };
        SDL_FlipMode m_flipMode{};
        float m_degrees{};
        uint32_t m_instanceCount{ 1 };

    };
#pragma endregion RenderComponent

#pragma region DebugComponent
    // Interface for components to provide debug rendering
    class DebugComponent : public Component
    {
    public:
        explicit DebugComponent(GameObject& owner, Renderer&) noexcept;
        ~DebugComponent() override;
        DebugComponent(DebugComponent&&) noexcept = delete;
        DebugComponent(DebugComponent const&) noexcept = delete;
        DebugComponent& operator=(DebugComponent const&) noexcept = delete;
        DebugComponent& operator=(DebugComponent&&) noexcept = delete;

        virtual void DebugRender() = 0;

    private:
        std::function<void()> const m_debugRenderFunction{ [this]{this->DebugRender();} };
        Renderer& m_renderer;
    };
#pragma endregion DebugComponent

#pragma region TextComponent
    class TextComponent final : public Component {
    public:
        explicit TextComponent(GameObject &owner,
                               std::string_view text, Font* pFont,
                               SDL_Color const&color = {255, 255, 255, 255}) noexcept;
        void SetFont(Font* pFont);
        void SetText(std::string_view text);
        void SetColor(SDL_Color const& color);

    private:
        std::string m_text;
        Font* m_pFont;
        std::unique_ptr<Sprite> m_pTexture{};
        SDL_Color m_color{ 255, 255, 255, 255 };
        RenderComponent& m_renderComponent;

        // Separate from UpdateTexture(), to initialize RenderComponent
        [[nodiscard]] Sprite* GetUpdatedTexture();
        void UpdateTexture();
    };
#pragma endregion TextComponent

#pragma region FPSComponent
    class FPSComponent final : public Component {
    public:
        explicit FPSComponent(GameObject &owner, Font* pFont,
                               SDL_Color const& color = {255, 255, 255, 255}) noexcept;
        void Update() noexcept override;
    };
#pragma endregion FPSComponent

#pragma region OrbitComponent
    class OrbitComponent final : public Component
    {
    public:
        /**
         * Rotates the object around @center at the @distance with the speed of radians.
         * Sets the transform value to transform component
         * The distance between the component's parent and parent of the parent is preserved
         * @param owner GameObject the component belongs to
         * @param radiansSec speed, with which the object rotates around the pivot point
         * @note Requires the owner to have a parent
         */
        explicit OrbitComponent(GameObject &owner, float radiansSec) noexcept;
        void Update() noexcept override;

    private:
        float m_radiansSec{ 0.25f * glm::pi<float>() };
    };
#pragma endregion OrbitComponent

}

#endif// ENGINE_COMPONENTS
