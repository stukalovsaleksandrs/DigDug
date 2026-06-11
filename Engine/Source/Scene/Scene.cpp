#include "Scene/Scene.hpp"

void Engine::Scene::Update()
{
    hierarchyElement.UpdateChildren();
    hierarchyElement.DeleteMarkedGameObjects();
}

Engine::GameObject& Engine::Scene::CreateGameObject(glm::vec2 const worldTopLeft) noexcept
{
    return *hierarchyElement.AddChild( std::make_unique<GameObject>(*this, worldTopLeft));
}

Engine::GameObject& Engine::Scene::CreateGameObject(GameObject& parent, glm::vec2 const worldTopLeft,
    bool const keepWorldLocation) noexcept
{
    auto& gameObject{ CreateGameObject(worldTopLeft) };
    gameObject.hierarchyElement.SetParent(parent.hierarchyElement, keepWorldLocation);
    return gameObject;
}
