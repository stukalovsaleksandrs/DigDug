#ifndef SE_SCENE_MANAGER_H
#define SE_SCENE_MANAGER_H

// Engine
#include "Engine/Scene/Scene.h"
#include "Engine/Core/Singleton.h"
// Standard
#include <vector>
#include <memory>

namespace Engine
{
    class Scene;
    class SceneManager final : public Singleton<SceneManager>
    {
    public:
        Scene& CreateScene();

        void Update();

    private:
        friend class Singleton;
        SceneManager() = default;
        std::vector<std::unique_ptr<Scene>> m_scenes{};

    };
}

#endif
