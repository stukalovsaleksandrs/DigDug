#ifndef SE_SCENE_H
#define SE_SCENE_H
// Engine
#include "Engine/Scene/GameObject.h"
#include "Engine/Scene/HierarchyElement.h"

namespace Engine
{
    class Scene final
    {
    public:
        // TODO: Wrap only the appropriate interface
        HierarchyElement hierarchyElement{nullptr, nullptr};
        void Update() const;

        GameObject& CreateGameObject(glm::vec2 localPosition) noexcept;

        GameObject& CreateGameObject(GameObject& parent, glm::vec2 localPosition, bool keepWorldPosition = false) noexcept;

    private:
        friend class SceneManager;
    };

}

#endif// SE_SCENE_H
