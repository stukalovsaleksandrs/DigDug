// Project
#include "Utils/Utils.h"
#include "Rendering/Renderer.h"
#include "Rendering/Texture2D.h"
#include "Components/Components.h"
// Third-party
#include <imgui.h>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_sdlrenderer3.h>
#include <implot.h>
// Standard
#include <iostream>
#include <chrono>

void Engine::Renderer::Init(SDL_Window* pWindow, glm::uvec2 const logicalDims)
{
    // Creating renderer
    SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");
    m_pSDLRenderer = SDL_CreateRenderer(pWindow, nullptr);
    if (!m_pSDLRenderer)
    {
        std::cout << "Failed to create the renderer: " << SDL_GetError() << "\n";
        throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
    }

    // Setting logical representation
    Utils::Check(
        SDL_SetRenderLogicalPresentation(
            m_pSDLRenderer,
            static_cast<int>(logicalDims.x),
            static_cast<int>(logicalDims.y),
            SDL_LOGICAL_PRESENTATION_LETTERBOX
        ), "Failed creating logical representation of the window"
    );

    // Initializing ImGui
    InitializeImGui(pWindow);
}

void Engine::Renderer::Render() const
{
    // Setting up new ImGui frame
    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    // Clearing the background
    const auto&[r, g, b, a]{ GetBackgroundColor() };
    SDL_SetRenderDrawColor(m_pSDLRenderer, r, g, b, a);
    SDL_RenderClear(m_pSDLRenderer);

    // Calling the render functions
    for (auto* const pRenderFunction : m_pRenderFunctions) pRenderFunction->operator()();

    // Showing the new frame
    SDL_RenderPresent(m_pSDLRenderer);

    // Rendering ImGui
    ImGui::Render();
    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), m_pSDLRenderer);
}

void Engine::Renderer::Destroy()
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

void Engine::Renderer::RenderTexture(Texture2D const& texture, glm::vec2 const location) const
{
    SDL_FRect destination{};
    destination.x = location.x;
    destination.y = location.y;
    SDL_GetTextureSize(texture.GetSDLTexture(), &destination.w, &destination.h);
    SDL_RenderTexture(m_pSDLRenderer, texture.GetSDLTexture(), nullptr, &destination);
}

void Engine::Renderer::RenderTexture(Texture2D const& texture, glm::vec2 const location, glm::vec2 const dimensions) const
{
    SDL_FRect destination{};
    destination.x = location.x;
    destination.y = location.y;
    destination.w = dimensions.x;
    destination.h = dimensions.y;
    SDL_RenderTexture(m_pSDLRenderer, texture.GetSDLTexture(), nullptr, &destination);
}

void Engine::Renderer::RegisterFunction(RenderFunctionType const& renderFunctionToAdd)
{
    m_pRenderFunctions.push_back(&renderFunctionToAdd);
}

void Engine::Renderer::UnregisterFunction(RenderFunctionType const& renderFunctionToRemove)
{
    // NOTE: [[maybe_unused]] is added to avoid unused variable errors in release build
    [[maybe_unused]] auto const erasedElementCount{
        std::erase(m_pRenderFunctions, &renderFunctionToRemove)
    };
    assert(erasedElementCount > 0 && "Render component not found");
}

void Engine::Renderer::InitializeImGui(SDL_Window* pWindow)
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

    ImGui_ImplSDL3_InitForSDLRenderer(pWindow, m_pSDLRenderer);
    ImGui_ImplSDLRenderer3_Init(m_pSDLRenderer);
}
