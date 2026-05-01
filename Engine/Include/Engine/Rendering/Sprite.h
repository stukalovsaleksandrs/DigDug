#ifndef ENGINE_SPRITE_H
#define ENGINE_SPRITE_H

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
            std::optional<SDL_FRect> srcRect{};// TODO: Remove std::optional
        };

        [[nodiscard]] SDL_Texture* GetSDLTexture() const;
        explicit Sprite(SDL_Texture* pTexture);
        explicit Sprite(std::string_view fullPath);
        ~Sprite();

        [[nodiscard]] glm::ivec2 GetDims() const;

        Sprite(const Sprite &) = delete;
        Sprite(Sprite &&) = delete;
        Sprite & operator= (Sprite const&) = delete;
        Sprite & operator= (Sprite const&&) = delete;

    private:
        SDL_Texture* m_pTexture;

    };
}

#endif// ENGINE_SPRITE_H
