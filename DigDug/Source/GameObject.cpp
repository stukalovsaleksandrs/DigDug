#include "GameObject.h"

#include <iostream>

void DAE::GameObject::Update() {
    DeleteMarkedComponents();

    // Updating components
    for (const auto& [pComponent, markedForDeletion] : m_components) {
        pComponent->Update();
    }
}

void DAE::GameObject::DeleteMarkedComponents() noexcept
{
    if (!m_anyComponentsToDelete) return;
    m_anyComponentsToDelete = false;
    std::erase_if(m_components,
        [](DeletableComponent const& component)
        {
            return component.markedForDeletion;
        }
    );
}
