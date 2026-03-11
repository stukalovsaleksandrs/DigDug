#include "Renderer.h"
#include "SceneManager.h"
#include "Texture2D.h"
#include "Utils.h"
#include "Components/Component.h"
#include <imgui.h>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_sdlrenderer3.h>
#include <implot.h>
#include <iostream>
#include <chrono>

void DAE::Renderer::Init(SDL_Window* pWindow)
{
    m_pWindow = pWindow;
    SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");
    m_pSDLRenderer = SDL_CreateRenderer(pWindow, nullptr);
    if (!m_pSDLRenderer)
    {
        std::cout << "Failed to create the renderer: " << SDL_GetError() << "\n";
        throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
    }

    InitializeImGui();
}

void DAE::Renderer::Render() const
{
    // Clearing the background
    const auto&[r, g, b, a]{ GetBackgroundColor() };
    SDL_SetRenderDrawColor(m_pSDLRenderer, r, g, b, a);
    SDL_RenderClear(m_pSDLRenderer);

    // Rendering the render components
    for (auto const pRenderComponent : m_pRenderComponents) {
    pRenderComponent->Render();
    }

    // Rendering debug components
    for (auto const pDebugComponent : m_pDebugComponents)
    {
        pDebugComponent->DebugRender(m_pSDLRenderer);
    }

    SDL_RenderPresent(m_pSDLRenderer);
}

void DAE::Renderer::Destroy()
{
    // Shutting down ImPlot
    ImPlot::DestroyContext();// Must preceed ImGui shutdown

    // Shutting down ImGui
    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    // Destroying the renderer
    if (m_pSDLRenderer)
    {
        SDL_DestroyRenderer(m_pSDLRenderer);
        m_pSDLRenderer = nullptr;
    }
}

void DAE::Renderer::RenderTexture(Texture2D const& texture, glm::vec2 const location) const
{
    SDL_FRect destination{};
    destination.x = location.x;
    destination.y = location.y;
    SDL_GetTextureSize(texture.GetSDLTexture(), &destination.w, &destination.h);
    SDL_RenderTexture(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &destination);
}

void DAE::Renderer::RenderTexture(Texture2D const& texture, glm::vec2 const location, glm::vec2 const dimensions) const
{
    SDL_FRect destination{};
    destination.x = location.x;
    destination.y = location.y;
    destination.w = dimensions.x;
    destination.h = dimensions.y;
    SDL_RenderTexture(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &destination);
}

SDL_Renderer* DAE::Renderer::GetSDLRenderer() const
{
    return m_pSDLRenderer;
}

void DAE::Renderer::RegisterComponent(Components::RenderComponent* pRenderComponent)
{
    m_pRenderComponents.push_back(pRenderComponent);
}

void DAE::Renderer::RegisterComponent(Components::DebugComponent* pDebugComponent)
{
    m_pDebugComponents.push_back(pDebugComponent);
}

void DAE::Renderer::UnregisterComponent(Components::RenderComponent* renderComponentToRemove)
{
    // NOTE: [[maybe_unused]] is added to avoid unused variable errors in release build
    [[maybe_unused]] auto const erasedElementCount{
        std::erase_if(m_pRenderComponents, [renderComponentToRemove](Components::RenderComponent const* currentRenderComponent)
        {
            return currentRenderComponent == renderComponentToRemove;
        })
    };
    // NOTE: All the render components must be present in the list, so
    // if one does not, it is an error. If the same component is
    // removed twice, it is also an error since the method should only be called
    // in the destructor of RenderComponent
    assert(erasedElementCount > 0 && "Render component not found");
}

void DAE::Renderer::InitializeImGui()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();// Must be after the ImGui::CreateContext()
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
#if __EMSCRIPTEN__
    // For an Emscripten build we are disabling file-system access, so let's not attempt to do a fopen() of the imgui.ini file.
    // You may manually call LoadIniSettingsFromMemory() to load settings from your own storage.
    io.IniFilename = NULL;
#endif

    ImGui_ImplSDL3_InitForSDLRenderer(m_pWindow, m_pSDLRenderer);
    ImGui_ImplSDLRenderer3_Init(m_pSDLRenderer);
}

