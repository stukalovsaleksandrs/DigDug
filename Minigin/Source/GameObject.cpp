#include "GameObject.h"

void DAE::GameObject::Update() {
    for (auto const& pComponent : m_pComponents) {
        pComponent->Update();
    }
}
