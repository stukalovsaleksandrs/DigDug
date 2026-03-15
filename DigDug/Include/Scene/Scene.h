#ifndef SCENE_H
#define SCENE_H
#include "GameObject.h"
#include "HierarchyElement.h"

namespace DAE
{
    class Scene final
    {
    public:
        HierarchyElement hierarchyElement{nullptr, nullptr};
        void Update() const;

        GameObject* CreateGameObject(glm::vec2 localPosition) noexcept;

        GameObject* CreateGameObject(GameObject& parent, glm::vec2 localPosition, bool keepWorldPosition = false) noexcept;

    private:
        friend class SceneManager;
    };

}

#endif
