#ifndef CACHETHRASHINGCOMPONENT_H
#define CACHETHRASHINGCOMPONENT_H
#include "Components/Components.h"
#include <algorithm>
#include <chrono>
#include <memory>

namespace DAE::Components
{
    class CacheThrashingComponent final : public DebugComponent
    {
    public:
        explicit CacheThrashingComponent(GameObject& owner);

        void DebugRender(SDL_Renderer* pSDLRenderer) override;

    private:
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
            int id{};
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
    void CacheThrashingComponent::RecalculatePlotData(int const sampleCount, std::vector<T>& buffer, std::vector<unsigned>& averageDurations)
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
#endif // CACHETHRASHINGCOMPONENT_H
