#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H
#include <filesystem>
#include <string_view>
#include <memory>
#include <map>
#include "Singleton.h"

namespace DAE
{
    class Texture2D;
    class Font;
    class ResourceManager final : public Singleton<ResourceManager>
    {
    public:
        void Init(std::filesystem::path const& dataPath);
        [[nodiscard]] std::shared_ptr<Texture2D> LoadTexture(std::string_view fileName);
        [[nodiscard]] std::shared_ptr<Font> LoadFont(std::string_view file, uint8_t size);

    private:
        friend class Singleton<ResourceManager>;
        ResourceManager() = default;
        std::filesystem::path m_dataPath;

        void UnloadUnusedResources();

        std::map<std::string, std::shared_ptr<Texture2D>> m_loadedTextures;
        std::map<std::pair<std::string, uint8_t>, std::shared_ptr<Font>> m_loadedFonts;

    };
}

#endif
