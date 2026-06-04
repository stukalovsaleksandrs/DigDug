#ifndef ENGINE_SPRITE
#define ENGINE_SPRITE

// Third-party
#include <glm/vec2.hpp>
#include <SDL3/SDL_rect.h>
// Standard
#include <optional>
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
                , srcRect{ pSprite->GetFullBounds() }
            {}

            View(Sprite* pSprite, SDL_FRect const& srcRect)
                : pSprite{ pSprite }
                , srcRect{ srcRect }
            {}

        };

        explicit Sprite(SDL_Texture* pTexture);
        explicit Sprite(std::string_view fullPath);
        ~Sprite();
        Sprite(const Sprite &) = delete;
        Sprite(Sprite &&) = delete;
        Sprite & operator= (Sprite const&) = delete;
        Sprite & operator= (Sprite const&&) = delete;

        [[nodiscard]] SDL_Texture* GetSDLTexture() const;
        [[nodiscard]] glm::ivec2 GetDims() const;
        [[nodiscard]] SDL_FRect GetFullBounds() const;

    private:
        SDL_Texture* m_pTexture;

    };
}

#endif// ENGINE_SPRITE
