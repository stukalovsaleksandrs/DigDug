#ifndef ENGINE_SCENE
#define ENGINE_SCENE
// Engine
#include "Engine/Scene/GameObject.hpp"
#include "Engine/Scene/HierarchyElement.hpp"

namespace Engine
{
    class Scene final
    {
    public:
        // TODO: Wrap only the appropriate interface
        HierarchyElement hierarchyElement{nullptr, nullptr};
        void Update();

        GameObject& CreateGameObject(glm::vec2 worldTopLeft) noexcept;

        GameObject& CreateGameObject(GameObject& parent, glm::vec2 localTopLeft = {}, bool keepWorldLocation = false) noexcept;

    private:
        friend class SceneManager;

    };

}

#endif// ENGINE_SCENE
