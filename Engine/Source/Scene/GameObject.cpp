#include "Scene/Scene.hpp"

Engine::GameObject::GameObject(Scene& scene, glm::vec2 const localPosition) noexcept
    : hierarchyElement(&scene.hierarchyElement, this)
      , m_localPosition{ localPosition }
      , m_scene(scene)
{
    UpdateWorldPosition();
}

/*******************************************
 * Lifetime
 *******************************************/

void Engine::GameObject::Update() {
    if (not m_active) return;
    DeleteMarkedComponents();

    // Updating components
    for (const auto& [pComponent, markedForDeletion] : m_components) {
        pComponent->Update();
    }

    // Updating children
    hierarchyElement.UpdateChildren();
}

void Engine::GameObject::MarkForDeletion() noexcept
{
    m_markedForDeletion = true;
}

bool Engine::GameObject::IsMarkedForDeletion() const noexcept
{
    return m_markedForDeletion;
}

void Engine::GameObject::SetActive(bool const active) noexcept
{
    m_active = active;
    for (const auto& [pComponent, _]: m_components)
        pComponent->SetActive(active);
}

/*******************************************
 * Transform
 *******************************************/

void Engine::GameObject::SetLocalPosition(glm::vec2 const position) noexcept
{
    m_localPosition = position;
    SetPositionDirty();
}

glm::vec2 Engine::GameObject::GetLocalLocation() const noexcept
{
    return m_localPosition;
}

glm::vec2 Engine::GameObject::GetWorldLocation() noexcept
{
    if (m_positionIsDirty)
    {
        UpdateWorldPosition();
        m_positionIsDirty = false;
    }
    return m_worldPosition;
}

void Engine::GameObject::SetPositionDirty() noexcept
{
    m_positionIsDirty = true;
    for (auto const& pChild : hierarchyElement.GetChildrenGameObjects())
    {
        pChild->SetPositionDirty();
    }
}

void Engine::GameObject::UpdateWorldPosition() noexcept
{
    if (IsDirectChildOfScene())
    {
        m_worldPosition = m_localPosition;
    }
    else
    {
        m_worldPosition =
            hierarchyElement.GetParentGameObject()->GetWorldLocation()
            + m_localPosition;
    }
}

bool Engine::GameObject::IsDirectChildOfScene() const noexcept
{
    return hierarchyElement.GetParentHierarchyElement() == &m_scene.hierarchyElement;
}

void Engine::GameObject::DeleteMarkedComponents() noexcept
{
    if (!m_componentDeletionFlagsDirty) return;
    m_componentDeletionFlagsDirty = false;
    std::erase_if(m_components,
                  [](DeletableComponent const& component)
                  {
                      return component.markedForDeletion;
                  }
    );
}
