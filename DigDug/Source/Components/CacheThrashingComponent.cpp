#include "Components/CacheThrashingComponent.h"
#include "Rendering/Renderer.h"
#include <imgui.h>
#include <implot.h>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_sdlrenderer3.h>

DAE::Components::CacheThrashingComponent::CacheThrashingComponent(GameObject& owner)
    : DebugComponent(owner)
{
    // Registering ourselves to the renderer
    Renderer::GetInstance().RegisterComponent(this);

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

void DAE::Components::CacheThrashingComponent::DebugRender()
{
    DrawEx1();
    DrawEx2();
}

void DAE::Components::CacheThrashingComponent::DrawEx1()
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

void DAE::Components::CacheThrashingComponent::DrawEx2()
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
