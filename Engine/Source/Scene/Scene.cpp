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

Engine::GameObject& Engine::Scene::CreateGameObject(GameObject& parent, glm::vec2 const localTopLeft,
    bool const keepWorldLocation) noexcept
{
    auto& gameObject{ CreateGameObject(localTopLeft) };
    gameObject.hierarchyElement.SetParent(parent.hierarchyElement, keepWorldLocation);
    return gameObject;
}

void Engine::Scene::ClearGameObjects() noexcept
{
    for (auto const& child: hierarchyElement.GetChildren())
    {
        child->MarkForDeletion();
    }
}
