#include "Component.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "GameObject.h"

DAE::Components::RenderComponent::RenderComponent(GameObject &owner) noexcept : Component(owner) {
    m_pOwnerTransformComponent = owner.AddComponent<TransformComponent>();
}

void DAE::Components::RenderComponent::Render() const {
    Renderer::GetInstance().RenderTexture(*m_texture, m_pOwnerTransformComponent->GetTransform().GetLocation());
}

void DAE::Components::RenderComponent::SetTexture(std::string_view filename) {
    m_texture = ResourceManager::GetInstance().LoadTexture(filename);
}
