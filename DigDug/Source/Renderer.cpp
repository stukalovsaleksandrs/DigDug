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

#include "../../Build/Release/_deps/implot-src/implot.h"

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

    // Sizing the average duration
    m_averageDurationsEx1.resize(11);
    m_intBuffer.resize(10'000'000);
}

void DAE::Renderer::Render()
{
    // Clearing the background
    const auto&[r, g, b, a]{ GetBackgroundColor() };
    SDL_SetRenderDrawColor(m_pRenderer, r, g, b, a);
    SDL_RenderClear(m_pRenderer);

    // Rendering the render components
    for (auto const renderComponent : m_pRenderComponents) {
        renderComponent->Render();
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

void DAE::Renderer::DrawImgui()
{
    // Rendering ImGui
    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    if (ImGui::Begin("Exercise 1"))
    {
        ImGui::InputInt("samples", &m_sampleCount);

        // Recalculating the data, when button is pressed
        if (ImGui::Button("Thrash the cache"))
        {
            // Getting the step count
            int stepCount{};
            for (int stepSize{ 1 }; stepSize <= 1024; stepSize*=2, ++stepCount){}

            // Getting the data
            for (int sampleIdx{}; sampleIdx < m_sampleCount; ++sampleIdx)// Sorry, Tom
            {
                for (int stepSize{ 1 }, stepIdx{}; stepSize <= 1024; stepSize*=2, ++stepIdx)
                {
                    auto const start{ std::chrono::high_resolution_clock::now() };

                    for (std::size_t idx{}; idx < m_intBuffer.size(); idx += stepSize)
                    {
                        m_intBuffer[idx] *= 2;
                    }

                    auto const end{ std::chrono::high_resolution_clock::now() };
                    auto duration{ std::chrono::duration_cast<std::chrono::nanoseconds>(end - start) };
                    m_averageDurationsEx1[stepIdx] = duration.count();
                }
            }

            // Dividing all the samples by the sample count to get the average data
            std::ranges::transform(m_averageDurationsEx1, m_averageDurationsEx1.begin(), [this](int const sampleIdx){ return sampleIdx / m_sampleCount; });
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

    ImGui::Render();

    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), m_pRenderer);

    // Presenting the surface
    SDL_RenderPresent(m_pRenderer);
}
