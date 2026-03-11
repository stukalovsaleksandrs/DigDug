#include "Renderer.h"

#include <chrono>

#include "SceneManager.h"
#include "Texture2D.h"
#include "Utils.h"
#include "Component.h"
#include <imgui.h>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_sdlrenderer3.h>
#include <iostream>
#include "implot.h"

void DAE::Renderer::Init(SDL_Window* pWindow)
{
    m_pWindow = pWindow;
    SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");
    m_pRenderer = SDL_CreateRenderer(pWindow, nullptr);
    if (!m_pRenderer)
    {
        std::cout << "Failed to create the renderer: " << SDL_GetError() << "\n";
        throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
    }

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

    ImGui_ImplSDL3_InitForSDLRenderer(m_pWindow, m_pRenderer);
    ImGui_ImplSDLRenderer3_Init(m_pRenderer);

    // Resizing exercise containers
    size_t constexpr bufferSize{ 10'000'000 };
    size_t constexpr stepCount{ 11 };

    // Resizing the containers for exercise 1
    m_averageDurationsEx1.resize(stepCount);
    m_intBuffer.resize(bufferSize);

    // Resizing the containers for exercise 2
    m_averageDurationsGameObject3D.resize(stepCount);
    m_averageDurationsGameObject3DAlt.resize(stepCount);
    m_gameObjects3D.resize(bufferSize);
    m_gameObjects3DAlt.resize(bufferSize);
}

void DAE::Renderer::Render()
{
    // Clearing the background
    const auto&[r, g, b, a]{ GetBackgroundColor() };
    SDL_SetRenderDrawColor(m_pRenderer, r, g, b, a);
    SDL_RenderClear(m_pRenderer);

    // Rendering the render components
    for (auto const pRenderComponent : m_pRenderComponents) {
        pRenderComponent->Render();
    }

    // Rendering debug components
    for (auto pDebugComponent : m_pDebugComponents)
    {
        pDebugComponent->DebugRender();
    }

    DrawImgui();
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
    if (m_pRenderer)
    {
        SDL_DestroyRenderer(m_pRenderer);
        m_pRenderer = nullptr;
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

SDL_Renderer* DAE::Renderer::GetSDLRenderer() const { return m_pRenderer; }

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

void DAE::Renderer::DrawImgui()
{
    // Rendering ImGui
    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    DrawEx1();
    DrawEx2();

    ImGui::Render();

    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), m_pRenderer);

    // Presenting the surface
    SDL_RenderPresent(m_pRenderer);
}

void DAE::Renderer::DrawEx1()
{
    if (ImGui::Begin("Exercise 1"))
    {
        ImGui::InputInt("samples", &m_sampleCountEx1);

        // Recalculating the data, when button is pressed
        if (ImGui::Button("Thrash the cache"))
        {
            RecalculatePlotData(m_sampleCountEx1, m_intBuffer, m_averageDurationsEx1);
        }

        // If vector is not empty, plotting it
        if (!m_averageDurationsEx1.empty())
        {
            ImPlot::SetNextAxesToFit();
            if (ImPlot::BeginPlot("Exercise 1"))
            {
                ImPlot::PlotLine<uint32_t>("Exercise 1", m_averageDurationsEx1.data(), static_cast<int>(m_averageDurationsEx1.size()));
                ImPlot::EndPlot();
            }
        }
    }
    ImGui::End();
}

void DAE::Renderer::DrawEx2()
{
    if (ImGui::Begin("Exercise 2"))
    {
        ImGui::InputInt("samples", &m_sampleCountEx2);

        // Recalculating the data, when button is pressed
        if (ImGui::Button("Thrash the cache with GameObject3D"))
        {
            RecalculatePlotData(m_sampleCountEx2, m_gameObjects3D, m_averageDurationsGameObject3D);
        }

        // Recalculating the data, when button is pressed
        if (ImGui::Button("Thrash the cache with GameObject3DAlt"))
        {
            RecalculatePlotData(m_sampleCountEx2, m_gameObjects3DAlt, m_averageDurationsGameObject3DAlt);
        }

        // If vector is not empty, plotting it
        ImPlot::SetNextAxesToFit();
        if (ImPlot::BeginPlot("Exercise 2"))
        {
            if (!m_averageDurationsGameObject3D.empty())
            {
                ImPlot::PlotLine<uint32_t>("GameObject3D", m_averageDurationsGameObject3D.data(), static_cast<int>(m_averageDurationsGameObject3D.size()));
            }
            if (!m_averageDurationsGameObject3DAlt.empty())
            {
                ImPlot::PlotLine<uint32_t>("GameObject3DAlt", m_averageDurationsGameObject3DAlt.data(), static_cast<int>(m_averageDurationsGameObject3D.size()));
            }
            ImPlot::EndPlot();
        }
    }

    ImGui::End();
}
