#include "Scene/Scene.h"

void DAE::Scene::Update() const
{
    hierarchyElement.UpdateChildren();
}

DAE::GameObject* DAE::Scene::CreateGameObject(glm::vec2 const localPosition) noexcept
{
    return hierarchyElement.AddChild( std::make_unique<GameObject>(*this, localPosition));
}

DAE::GameObject* DAE::Scene::CreateGameObject(GameObject& parent, glm::vec2 const localPosition,
    bool const keepWorldPosition) noexcept
{
    auto const gameObject{ CreateGameObject(localPosition) };
    gameObject->hierarchyElement.SetParent(parent.hierarchyElement, keepWorldPosition);
    return gameObject;
}
