#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H
#include <vector>
#include <memory>
#include "Scene.h"
#include "../Core/Singleton.h"

namespace DAE
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
