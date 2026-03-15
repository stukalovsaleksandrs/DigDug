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
        void Update() const
        {
            hierarchyElement.UpdateChildren();
        }

        GameObject* CreateGameObject(glm::vec2 const localPosition) noexcept
        {
            return hierarchyElement.AddChild( std::make_unique<GameObject>(*this, localPosition));
        }

        GameObject* CreateGameObject(GameObject& parent, glm::vec2 const localPosition, bool const keepWorldPosition = false) noexcept
        {
            auto const gameObject{ CreateGameObject(localPosition) };
            gameObject->hierarchyElement.SetParent(parent.hierarchyElement, keepWorldPosition);
            return gameObject;
        }

    private:
        friend class SceneManager;
    };

}

#endif
