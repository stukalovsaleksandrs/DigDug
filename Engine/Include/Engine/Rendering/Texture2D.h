#ifndef ENGINE_TEXTURE2D_H
#define ENGINE_TEXTURE2D_H

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
    class Texture2D final
    {
    public:
        struct Data final
        {
            Texture2D* pTexture{};
            std::optional<SDL_FRect> const srcRect{};
        };

        [[nodiscard]] SDL_Texture* GetSDLTexture() const;
        explicit Texture2D(SDL_Texture* pTexture);
        explicit Texture2D(std::string_view fullPath);
        ~Texture2D();

        [[nodiscard]] glm::ivec2 GetDims() const;

        Texture2D(const Texture2D &) = delete;
        Texture2D(Texture2D &&) = delete;
        Texture2D & operator= (Texture2D const&) = delete;
        Texture2D & operator= (Texture2D const&&) = delete;

    private:
        SDL_Texture* m_pTexture;

    };
}

#endif// ENGINE_TEXTURE2D_H
