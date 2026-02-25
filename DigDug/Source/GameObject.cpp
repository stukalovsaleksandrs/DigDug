#include "GameObject.h"

#include <iostream>

void DAE::GameObject::Update() {
    DeleteMarkedComponents();

    // Updating components
    for (const auto& [pComponent, markedForDeletion] : m_components) {
        pComponent->Update();
    }
}

void DAE::GameObject::SetParent(GameObject* pParent) noexcept
{
    // 1. Checking if the new parent is valid
    // Not nullptr, not current parent
    if (IsChild(pParent) || pParent == this || m_pParent != pParent ) return;
    // 2. Removing ourselves from the parent
    RemoveChild(pParent);
    // 3. Setting the new parent
    m_pParent = pParent;
    // 4. Adding ourselves to the parent's children list
    pParent->AddChild(this);
    // 5. Updating transform
    UpdateTransform();
}

bool DAE::GameObject::IsChild(GameObject* pChild) const noexcept
{
    if (pChild == this || !pChild) return false;
    return std::ranges::binary_search(m_pChildren, pChild);
}

void DAE::GameObject::RemoveChild(GameObject* pChild) noexcept
{
    // 1. Checking if the new child is valid
    if (!(IsChild(pChild) && pChild)) return;
    // 2. Removing the child given from its parent's children list
    if (m_pParent)
    {
        assert(m_pParent->IsChild(this));
        std::erase(m_pParent->m_pChildren, this);
    }
    // 3. Removing ourselves as a parent of the child
    pChild->m_pParent = nullptr;
    // 4. Updating transform
    UpdateTransform();
}

void DAE::GameObject::AddChild(GameObject* pChild) noexcept
{
    // 1. Checking if the new child is valid
    if (IsChild(pChild) || !pChild) return;
    // 2. Removing the child from the previous parent's children list
    pChild->m_pParent->RemoveChild(pChild);
    // 3. Setting ourselves as parent of the new child
    pChild->m_pParent = this;
    // 4. Adding the child to the children list
    m_pChildren.push_back(pChild);
    // 5. Updating our transform
    UpdateTransform();
}

DAE::GameObject* DAE::GameObject::GetChild(unsigned int const idx) const
{
    return m_pChildren.at(idx);
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

void DAE::GameObject::UpdateTransform() noexcept
{
    throw std::logic_error("Not implemented yet");
}
