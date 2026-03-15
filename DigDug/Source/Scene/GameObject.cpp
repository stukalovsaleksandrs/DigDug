#include "Scene/Scene.h"

DAE::GameObject::GameObject(Scene& scene, glm::vec2 const localPosition) noexcept
    : hierarchyElement(&scene.hierarchyElement, nullptr)
    , m_localPosition{ localPosition }
    , m_scene(scene)
{
    UpdateWorldPosition();
}

void DAE::GameObject::Update() {
    DeleteMarkedComponents();

    // Updating components
    for (const auto& [pComponent, markedForDeletion] : m_components) {
        pComponent->Update();
    }

    // Updating children
    hierarchyElement.UpdateChildren();
}

void DAE::GameObject::DeleteMarkedComponents() noexcept
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

void DAE::GameObject::SetLocalPosition(glm::vec2 position) noexcept
{
    m_localPosition = position;
    SetPositionDirty();
}

glm::vec2 DAE::GameObject::GetLocalPosition() const noexcept
{
    return m_localPosition;
}

glm::vec2 DAE::GameObject::GetWorldPosition() noexcept
{
    if (m_positionIsDirty)
    {
        UpdateWorldPosition();
        m_positionIsDirty = false;
    }
    return m_worldPosition;
}

void DAE::GameObject::UpdateWorldPosition() noexcept
{
    if (IsDirectChildOfScene())
    {
        m_worldPosition = m_localPosition;
    }
    else
    {
        m_worldPosition =
            hierarchyElement.GetParentGameObject()->GetWorldPosition()
            + m_localPosition;
    }
}

bool DAE::GameObject::IsDirectChildOfScene() noexcept
{
    return hierarchyElement.GetParentHierarchyElement() == &m_scene.hierarchyElement;
}

void DAE::GameObject::SetPositionDirty() noexcept
{
    m_positionIsDirty = true;
    for (auto const& pChild : hierarchyElement.GetChildrenGameObjects())
    {
        pChild->SetPositionDirty();
    }
}
