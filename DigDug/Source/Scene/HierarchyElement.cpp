#include "Scene/HierarchyElement.h"
#include "Scene/Scene.h"

DAE::HierarchyElement::HierarchyElement(HierarchyElement* pSceneHierarchyElement, GameObject* pOwnerGameObject) noexcept
    : m_pParentHierarchyElement(pSceneHierarchyElement)
    , m_pSceneHierarchyElement(pSceneHierarchyElement)
    , m_pOwnerGameObject(pOwnerGameObject)
{
}

void DAE::HierarchyElement::SetParent(HierarchyElement& newParent, bool const keepWorldPosition) noexcept
{
    // 1. Validation
    // 1.1. Scene root actions
    if (!m_pParentHierarchyElement/*Attempt for reparenting a scene root*/)
    {
        assert(false && "Trying to perform actions on the scene root");
        return;
    }
    // 1.2. Parent validation
    if (IsChild(newParent.m_pOwnerGameObject)
        || &newParent == this
        || m_pParentHierarchyElement == &newParent)
    {
        return;
    }
    // 2. Updating transform
    // NOTE: Must be done before removal or addition of parents
    // because this element needs to know of its parent if it wants
    // to calculate the correct world position
    if (&newParent == m_pSceneHierarchyElement)
    {
        m_pOwnerGameObject->SetLocalPosition(m_pOwnerGameObject->GetWorldPosition());
    }
    else
    {
        if (keepWorldPosition)
        {
            m_pOwnerGameObject->SetLocalPosition(m_pOwnerGameObject->GetWorldPosition() - newParent.m_pOwnerGameObject->GetWorldPosition());
        }
        m_pOwnerGameObject->SetPositionDirty();
    }
    m_pOwnerGameObject->UpdateWorldPosition();
    // 3. Removing ourselves from the old parent and adding to the new one
    if (m_pParentHierarchyElement)
    {
        // Moving from a game object to a game object
        newParent.AddChild(m_pParentHierarchyElement->RemoveChild(this->m_pOwnerGameObject));
    }
    else if (m_pParentHierarchyElement)// pNewParent = nullptr()
    {
        // Moving from a game object to the scene
        AddChild(m_pParentHierarchyElement->RemoveChild(this->m_pOwnerGameObject));
    }
    // 4. Setting the new parent
    m_pParentHierarchyElement = &newParent;
}

auto GetIsEqual(DAE::GameObject const * const pGameObject)
{
    assert(pGameObject);
    return [pGameObject](std::unique_ptr<DAE::GameObject> const& child)
    {
        return pGameObject == child.get();
    };
}

bool DAE::HierarchyElement::IsChild(GameObject const* pChild) const noexcept
{
    if (pChild == m_pOwnerGameObject || !pChild) return false;
    // NOTE: Not using ranges, because they disallow passing custom predicates
    auto const it{ std::ranges::find_if(m_pChildren, GetIsEqual(pChild)) };
    return it != m_pChildren.end();
}

std::unique_ptr<DAE::GameObject> DAE::HierarchyElement::RemoveChild(
    GameObject const* pChildToRemove) noexcept
{
    // 1. Checking if the new child is valid
    if (pChildToRemove) return nullptr;

    // 2. Retrieving an iterator to the child
    auto const childIterator{
        std::ranges::find_if(m_pChildren,GetIsEqual(pChildToRemove))
    };
    if (childIterator == m_pChildren.end()) return nullptr;// Child is absent

    // 3. Transferring the ownership over the child to a temporary
    std::unique_ptr<GameObject> removedChild = std::move(*childIterator);

    // 4. Removing the child
    m_pChildren.erase(childIterator);

    // 5. Removing ourselves as the parent
    removedChild->hierarchyElement.m_pParentHierarchyElement = nullptr;

    return removedChild;// Works thanks to copy elision
}

DAE::GameObject* DAE::HierarchyElement::AddChild(std::unique_ptr<GameObject>&& pNewChild) noexcept
{
    // 1. Checking if the new child is valid
    if (IsChild(pNewChild.get()) || !pNewChild) return nullptr;
    // 2. Removing the child from the previous parent's children list
    pNewChild->hierarchyElement.m_pParentHierarchyElement->RemoveChild(pNewChild.get());
    // 3. Setting ourselves as parent of the new child
    pNewChild->hierarchyElement.m_pParentHierarchyElement = this;
    // 4. Adding the child to the children list
    m_pChildren.push_back(std::move(pNewChild));
    // 5. Returning non-owning pointer
    return m_pChildren.back().get();
}

DAE::GameObject* DAE::HierarchyElement::GetOwnerGameObject() noexcept
{
    return m_pOwnerGameObject;
}

std::vector<std::unique_ptr<DAE::GameObject>>& DAE::HierarchyElement::GetChildrenGameObjects() noexcept
{
    return m_pChildren;
}

void DAE::HierarchyElement::UpdateChildren() const noexcept
{
    for (auto& pGameObject: m_pChildren)
    {
        pGameObject->Update();
    }
}

void DAE::HierarchyElement::DeleteMarkedGameObjects() noexcept
{
    // Deleting the direct children
    std::erase_if(m_pChildren, [](std::unique_ptr<GameObject> const& pGameObject)
    {
        return pGameObject->IsMarkedForDeletion();
    });

    // Deleting the indirect children
    for (auto const& pGameObject: m_pChildren)
    {
        pGameObject->hierarchyElement.DeleteMarkedGameObjects();
    }
}
