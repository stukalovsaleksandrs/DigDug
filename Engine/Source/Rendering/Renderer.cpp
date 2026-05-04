// Project
#include "Utils/Utils.h"
#include "Rendering/Renderer.h"
#include "Rendering/Sprite.h"
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

void Engine::Renderer::RenderTexture(Sprite const& texture, glm::vec2 const location) const noexcept
{
    SDL_FRect destination{};
    destination.x = location.x;
    destination.y = location.y;
    SDL_GetTextureSize(texture.GetSDLTexture(), &destination.w, &destination.h);
    SDL_RenderTexture(m_pSDLRenderer, texture.GetSDLTexture(), nullptr, &destination);
}

void Engine::Renderer::RenderTexture(Sprite const& texture, SDL_FRect const& srcRect, SDL_FRect const& dstRect, float const degrees, SDL_FlipMode const flipMode) const noexcept
{
    SDL_RenderTextureRotated(m_pSDLRenderer, texture.GetSDLTexture(), &srcRect, &dstRect, degrees, nullptr, flipMode);
}

void Engine::Renderer::RenderLine(glm::vec2 const p1, glm::vec2 const p2) const noexcept
{
    SDL_SetRenderDrawColor(m_pSDLRenderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderLine(m_pSDLRenderer, p1.x, p1.y, p2.x, p2.y);
}

void Engine::Renderer::RegisterFunction(RenderFunctionType const& renderFunctionToAdd) noexcept
{
    m_pRenderFunctions.push_back(&renderFunctionToAdd);
}

void Engine::Renderer::UnregisterFunction(RenderFunctionType const& renderFunctionToRemove) noexcept
{
    // NOTE: [[maybe_unused]] is added to avoid unused variable errors in release build
    [[maybe_unused]] auto const erasedElementCount{
        std::erase(m_pRenderFunctions, &renderFunctionToRemove)
    };
    assert(erasedElementCount > 0 && "Render component not found");
}

void Engine::Renderer::InitializeImGui(SDL_Window* pWindow) noexcept
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
