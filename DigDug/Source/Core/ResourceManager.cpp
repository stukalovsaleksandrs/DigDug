#include "Core/ResourceManager.h"
#include "Rendering/Texture2D.h"
#include "Rendering/Font.h"
#include "Utils/Utils.h"
#include <SDL3_ttf/SDL_ttf.h>
#include <format>

namespace fs = std::filesystem;

void DAE::ResourceManager::Init(std::filesystem::path const& dataPath)
{
    m_dataPath = dataPath;

    if (!TTF_Init())
    {
        Utils::ThrowSDLError("Failed to load support for fonts");
    }
}

std::shared_ptr<DAE::Texture2D> DAE::ResourceManager::LoadTexture(std::string_view const fileName)
{
    auto const fullPath = m_dataPath/fileName;
    auto const filename = fs::path(fullPath).filename().string();
    if(m_loadedTextures.find(filename) == m_loadedTextures.end())
        m_loadedTextures.insert(std::pair(filename,std::make_shared<Texture2D>(fullPath.string())));
    return m_loadedTextures.at(filename);
}

std::shared_ptr<DAE::Font> DAE::ResourceManager::LoadFont(std::string_view const file, uint8_t size)
{
    auto const fullPath = m_dataPath/file;
    auto const filename = fs::path(fullPath).filename().string();
    auto const key = std::pair<std::string, uint8_t>(filename, size);
    if(m_loadedFonts.find(key) == m_loadedFonts.end())
        m_loadedFonts.insert(std::pair(key,std::make_shared<Font>(fullPath.string(), size)));
    return m_loadedFonts.at(key);
}

void DAE::ResourceManager::UnloadUnusedResources()
{
    for (auto it = m_loadedTextures.begin(); it != m_loadedTextures.end();)
    {
        if (it->second.use_count() == 1) {
            it = m_loadedTextures.erase(it);
        }
        else {
            ++it;
        }
    }

    for (auto it = m_loadedFonts.begin(); it != m_loadedFonts.end();)
    {
        if (it->second.use_count() == 1) {
            it = m_loadedFonts.erase(it);
        }
        else {
            ++it;
        }
    }
}
