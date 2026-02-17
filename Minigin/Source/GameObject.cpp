#include "GameObject.h"

#include <algorithm>

#include "ResourceManager.h"
#include "Renderer.h"

DAE::GameObject::~GameObject() = default;

void DAE::GameObject::Update(){}

void DAE::GameObject::Render() const
{
    auto const& location{ m_transform.GetLocation() };
    Renderer::GetInstance().RenderTexture(*m_texture, location);
}

void DAE::GameObject::SetTexture(std::string_view const filename)
{
    m_texture = ResourceManager::GetInstance().LoadTexture(filename);
}

void DAE::GameObject::SetLocation(glm::vec2 const location)
{
    m_transform.SetLocation({location.x, location.y, 0.f});
}

template<DAE::DerivedComponent ComponentType>
bool DAE::GameObject::AddComponent(ComponentType const& component) {
    if (HasComponent<ComponentType>()) return false;
    m_pComponents.emplace_back(std::make_unique<ComponentType>(component));
    return true;
}

template<DAE::DerivedComponent ComponentType>
bool DAE::GameObject::HasComponent() const {
    return std::ranges::any_of(m_pComponents, [](std::unique_ptr<Component> const& pComponent) {
        return dynamic_cast<ComponentType*>(pComponent.get) != nullptr;
    });
}

template<DAE::DerivedComponent ComponentType>
void DAE::GameObject::RemoveComponent(ComponentType const&) {
    std::erase_if(m_pComponents, [](auto const& pComponent) {
        return dynamic_cast<ComponentType*>(pComponent.get) != nullptr;
    });
}
