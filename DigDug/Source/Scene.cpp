#include <algorithm>
#include "Scene.h"

void DAE::Scene::Update() const
{
    hierarchyElement.UpdateChildren();
}
