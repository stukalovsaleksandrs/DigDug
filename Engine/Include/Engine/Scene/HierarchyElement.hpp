#ifndef ENGINE_HIERARCHY_ELEMENT
#define ENGINE_HIERARCHY_ELEMENT

// Standard
#include <memory>
#include <vector>

namespace Engine
{
    class GameObject;

    // An object managing parent/child logic
    class HierarchyElement
    {
    public:
        // NOTE: First argument is nullptr for the scene, the second is nullptr for the scene's direct children
        explicit HierarchyElement(HierarchyElement* pSceneHierarchyElement, GameObject* pOwnerGameObject) noexcept;
        // NOTE: Deleting the copy semantics, because std::unique_ptr is not copyable
        HierarchyElement(const HierarchyElement&) = delete;
        HierarchyElement& operator=(const HierarchyElement&) = delete;

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

        void DeleteMarkedGameObjects() noexcept;

    private:
        HierarchyElement* m_pParentHierarchyElement{};
        HierarchyElement* const m_pSceneHierarchyElement{};
        GameObject* m_pOwnerGameObject{};
        std::vector<std::unique_ptr<GameObject>> m_pChildren{};

        std::unique_ptr<GameObject> RemoveChild(GameObject const* pChildToRemove) noexcept;
    };
}

#endif// ENGINE_HIERARCHY_ELEMENT
