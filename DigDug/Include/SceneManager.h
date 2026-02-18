#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H
#include <vector>
#include <memory>
#include "Scene.h"
#include "Singleton.h"

namespace DAE
{
    class Scene;
    class SceneManager final : public Singleton<SceneManager>
    {
    public:
        Scene& CreateScene();

        void Update();
        void Render() const;
    private:
        friend class Singleton<SceneManager>;
        SceneManager() = default;
        std::vector<std::unique_ptr<Scene>> m_scenes{};
    };
}

#endif
