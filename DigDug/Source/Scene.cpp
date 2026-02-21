#include <algorithm>
#include "Scene.h"

void DAE::Scene::Add(std::unique_ptr<GameObject> object)
{
    assert(object && "Cannot add a null GameObject to the scene.");
    m_objects.emplace_back(std::move(object));
}

void DAE::Scene::Remove(GameObject const& object)
{
    std::erase_if(
        m_objects,
        [&object](auto const& ptr) { return ptr.get() == &object; }
    );
}

void DAE::Scene::RemoveAll()
{
    m_objects.clear();
}

void DAE::Scene::Update() const
{
    for(auto const& object : m_objects)
    {
        object->Update();
    }
}
