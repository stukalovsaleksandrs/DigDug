#include "Scene/Scene.h"

void Engine::Scene::Update() const
{
    hierarchyElement.UpdateChildren();
}

Engine::GameObject& Engine::Scene::CreateGameObject(glm::vec2 const localPosition) noexcept
{
    return *hierarchyElement.AddChild( std::make_unique<GameObject>(*this, localPosition));
}

Engine::GameObject& Engine::Scene::CreateGameObject(GameObject& parent, glm::vec2 const localPosition,
    bool const keepWorldPosition) noexcept
{
    auto& gameObject{ CreateGameObject(localPosition) };
    gameObject.hierarchyElement.SetParent(parent.hierarchyElement, keepWorldPosition);
    return gameObject;
}
