#ifndef HIERARCHY_ELEMENT_H
#define HIERARCHY_ELEMENT_H
#include <memory>
#include <vector>

namespace DAE
{
    class GameObject;

    // An object managing parent/child logic
    class HierarchyElement
    {
    public:
        // NOTE: First argument is nullptr for the scene, the second is nullptr for the scene's direct children
        explicit HierarchyElement(HierarchyElement* pSceneHierarchyElement, GameObject* pOwnerGameObject) noexcept;

        // Attempts removing the current game object from
        // its parent and adding to the new parent.
        void SetParent(HierarchyElement& newParent, bool keepWorldPosition) noexcept;

        /**
         * @return Whether the pChild is a child of the current game object
         */
        bool IsChild(GameObject const * pChild) const noexcept;

        [[nodiscard]] GameObject* GetChild(unsigned int const idx) const{ return m_pChildren.at(idx).get(); };
        [[nodiscard]] HierarchyElement* GetParentHierarchyElement() const{ return m_pParentHierarchyElement; };
        [[nodiscard]] GameObject* GetParentGameObject() const{ return m_pParentHierarchyElement->GetOwnerGameObject(); };

        GameObject* AddChild(std::unique_ptr<GameObject>&&) noexcept;

        [[nodiscard]] GameObject* GetOwnerGameObject() noexcept;
        [[nodiscard]] std::vector<std::unique_ptr<GameObject>>& GetChildrenGameObjects() noexcept;

        void UpdateChildren() const noexcept;

    private:
        HierarchyElement* m_pParentHierarchyElement{};
        HierarchyElement* const m_pSceneHierarchyElement{};
        GameObject* m_pOwnerGameObject{};
        std::vector<std::unique_ptr<GameObject>> m_pChildren{};

        std::unique_ptr<GameObject> RemoveChild(GameObject const* pChildToRemove) noexcept;
    };
}

#endif
