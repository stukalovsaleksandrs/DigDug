#include "GameObject.h"

#include <algorithm>
#include <ranges>

#include "ResourceManager.h"
#include "Renderer.h"

DAE::GameObject::~GameObject() = default;

void DAE::GameObject::Update(){}

void DAE::GameObject::Render()
{
    auto const transformComponent{
        this->GetComponent<Components::TransformComponent>()
    };
    assert(transformComponent.has_value());
    Renderer::GetInstance().RenderTexture(*m_texture, transformComponent.value()->GetTransform().GetLocation());
}

void DAE::GameObject::SetTexture(std::string_view const filename)
{
    m_texture = ResourceManager::GetInstance().LoadTexture(filename);
}

