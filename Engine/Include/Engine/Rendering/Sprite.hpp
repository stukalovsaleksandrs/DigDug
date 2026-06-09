#ifndef ENGINE_SPRITE
#define ENGINE_SPRITE

// Engine
#include "Renderer.hpp"
// Third-party
#include <glm/vec2.hpp>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_blendmode.h>
#include <SDL3/SDL_render.h>
// Standard
#include <string>

struct SDL_Texture;
namespace Engine
{
    /**
     * Simple RAII wrapper for an SDL_Texture
     */
    class Sprite final
    {
    public:
        // A non-owning "reference" to a specific section of sprite
        // used mostly for tiles on a sprite sheet
        struct View final
        {
            Sprite* pSprite;// Because copy assignment is needed
            SDL_FRect srcRect;

            // Makes the view span the entire sprite
            explicit View(Sprite* pSprite)
                : pSprite{ pSprite }
                , srcRect{ pSprite->GetFullBounds() } {}

            View(Sprite* pSprite, SDL_FRect const& srcRect)
                : pSprite{ pSprite }
                , srcRect{ srcRect } {}
        };

        explicit Sprite(SDL_Texture* pTexture);
        explicit Sprite(std::string_view fullPath);
        // Creates a texture of the dims and color given
        explicit Sprite(SDL_Color const& color, glm::uvec2 dims, SDL_BlendMode blendMode = SDL_BLENDMODE_NONE);
        ~Sprite() noexcept;
        Sprite(Sprite const &) noexcept = delete;
        Sprite(Sprite &&) noexcept = delete;
        Sprite& operator= (Sprite const&) noexcept = delete;
        Sprite& operator= (Sprite const&&) noexcept = delete;

        [[nodiscard]] SDL_Texture* GetSDLTexture() const;
        [[nodiscard]] glm::ivec2 GetDims() const;
        [[nodiscard]] SDL_FRect GetFullBounds() const;

        void Render() const noexcept;
        void MaskCircle(Utils::Circle const&) const noexcept;
        void MaskSquare(Utils::Square const&) const noexcept;

    private:
        SDL_Renderer* m_pSDLRenderer{ Renderer::GetInstance().GetSDLRenderer() };
        SDL_Texture* m_pSDLTexture;
        SDL_BlendMode m_blendMode{};
    };
}

#endif// ENGINE_SPRITE
