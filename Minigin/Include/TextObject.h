#ifndef TEXT_OBJECT_H
#define TEXT_OBJECT_H

#include "GameObject.h"
#include "Transform.h"
#include <SDL3_ttf/SDL_ttf.h>
#include <string>
#include <memory>

namespace DAE
{
    class Font;
    class Texture2D;
    class TextObject final : public GameObject
    {
    public:
        TextObject(std::string_view text, std::shared_ptr<Font> font, SDL_Color const& color = { 255, 255, 255, 255 });
        ~TextObject() override = default;
        TextObject(TextObject const& other) = delete;
        TextObject(TextObject&& other) = delete;
        TextObject& operator=(TextObject const& other) = delete;
        TextObject& operator=(TextObject&& other) = delete;

        void Update() override;
        void Render() const override;

        void SetText(std::string_view);
        void SetLocation(glm::vec2) override;
        void SetColor(SDL_Color const&);

    private:
        bool m_needsUpdate{ true };
        std::string m_text{};
        SDL_Color m_color{ 255, 255, 255, 255 };
        Transform m_transform{};
        std::shared_ptr<Font> m_font{};
        std::shared_ptr<Texture2D> m_textTexture{};

    };
}

#endif
