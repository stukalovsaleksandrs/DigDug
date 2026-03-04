#ifndef RENDERER_H
#define RENDERER_H
#include <algorithm>
#include <chrono>
#include <memory>

#include "Singleton.h"
#include <SDL3/SDL.h>
#include <glm/vec2.hpp>
#include <vector>

namespace DAE
{
    namespace Components { class RenderComponent; }
    class Texture2D;
    /**
     * Simple RAII wrapper for the SDL renderer
     */
    class Renderer final : public Singleton<Renderer>
    {
    public:
        void Init(SDL_Window* pWindow);
        // Calls Render() on all the registered components
        void Render();
        void Destroy();

        void RenderTexture(Texture2D const& texture, glm::vec2 location) const;
        void RenderTexture(Texture2D const& texture, glm::vec2 location, glm::vec2 dimensions) const;

        [[nodiscard]] SDL_Renderer* GetSDLRenderer() const;

        [[nodiscard]] const SDL_Color& GetBackgroundColor() const { return m_clearColor; }
        void SetBackgroundColor(SDL_Color const& color) { m_clearColor = color; }

        // Adds component for tracking, so the renderer now renders it
        void RegisterComponent(Components::RenderComponent* renderComponent)
        {
            m_pRenderComponents.push_back(renderComponent);
        }

        // Removes the given component from tracking, so the renderer will not attempt to render it
        void UnregisterComponent(Components::RenderComponent* renderComponentToRemove)
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

    private:
        SDL_Renderer* m_pRenderer{};
        SDL_Window* m_pWindow{};
        SDL_Color m_clearColor{};
        std::vector<Components::RenderComponent*> m_pRenderComponents;// Non-owning
        int m_sampleCountEx1{ 10 };

        // Ex1
        std::vector<unsigned> m_averageDurationsEx1;
        std::vector<unsigned> m_intBuffer;

        // Ex2
        struct Transform
        {
            float matrix[16] = {
                1,0,0,0,
                0,1,0,0,
                0,0,1,0,
                0,0,0,1 };
        };

        class GameObject3D
        {
        public:
            Transform transform;
            int id;
        };

        class GameObject3DAlt
        {
        public:
            std::unique_ptr<Transform> transform;
            int id;
        };

        std::vector<unsigned> m_averageDurationsGameObject3D;
        std::vector<GameObject3D> m_gameObjects3D;

        std::vector<unsigned> m_averageDurationsGameObject3DAlt;
        std::vector<GameObject3DAlt> m_gameObjects3DAlt;
        int m_sampleCountEx2{ 10 };

        void DrawImgui();
        void DrawEx1();
        void DrawEx2();

        template<typename T>
        void RecalculatePlotData(int sampleCount, std::vector<T>& buffer, std::vector<unsigned>& averageDurations);
    };

    template <typename T>
    void Renderer::RecalculatePlotData(int const sampleCount, std::vector<T>& buffer, std::vector<unsigned>& averageDurations)
    {
        // Getting the step count
        int stepCount{};
        for (int stepSize{ 1 }; stepSize <= 1024; stepSize*=2, ++stepCount){}

        // Getting the data
        for (int sampleIdx{}; sampleIdx < sampleCount; ++sampleIdx)// Sorry, Tom
        {
            for (int stepSize{ 1 }, stepIdx{}; stepSize <= 1024; stepSize*=2, ++stepIdx)
            {
                auto const start{ std::chrono::high_resolution_clock::now() };

                for (std::size_t bufferIdx{}; bufferIdx < buffer.size(); bufferIdx += stepSize)
                {
                    if constexpr(requires{T::id;})
                    {
                        buffer[bufferIdx].id *= static_cast<int>(bufferIdx);
                    }
                    else
                    {
                        buffer[bufferIdx] *= 2;
                    }
                }

                auto const end{ std::chrono::high_resolution_clock::now() };
                auto duration{ std::chrono::duration_cast<std::chrono::nanoseconds>(end - start) };
                averageDurations[stepIdx] = static_cast<int>(duration.count());
            }
        }

        // Dividing all the samples by the sample count to get the average data
        std::ranges::transform(averageDurations, averageDurations.begin(), [this](int const sampleIdx){ return sampleIdx / m_sampleCountEx1; });
    }
}

#endif
