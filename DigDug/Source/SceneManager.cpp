#include "SceneManager.h"
#include "Scene.h"

void DAE::SceneManager::Update()
{
    for(auto const& scene : m_scenes)
    {
        scene->Update();
    }
}

DAE::Scene& DAE::SceneManager::CreateScene()
{
    m_scenes.emplace_back(new Scene());
    return *m_scenes.back();
}
