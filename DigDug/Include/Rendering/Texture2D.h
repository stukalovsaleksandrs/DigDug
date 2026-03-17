#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#include <glm/vec2.hpp>
#include <string>

struct SDL_Texture;
namespace DAE
{
    /**
     * Simple RAII wrapper for an SDL_Texture
     */
    class Texture2D final
    {
    public:
        SDL_Texture* GetSDLTexture() const;
        explicit Texture2D(SDL_Texture* pTexture);
        explicit Texture2D(std::string_view fullPath);
        ~Texture2D();

        glm::vec2 GetDims() const;

        Texture2D(const Texture2D &) = delete;
        Texture2D(Texture2D &&) = delete;
        Texture2D & operator= (Texture2D const&) = delete;
        Texture2D & operator= (Texture2D const&&) = delete;

    private:
        SDL_Texture* m_pTexture;

    };
}

#endif
