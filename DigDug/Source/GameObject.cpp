#include "GameObject.h"

void DAE::GameObject::Update() const {
    for (auto const& pComponent : m_pComponents) {
        pComponent->Update();
    }
}
