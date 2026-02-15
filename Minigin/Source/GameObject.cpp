#include "GameObject.h"
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
