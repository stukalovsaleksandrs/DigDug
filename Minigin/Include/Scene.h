#ifndef SCENE_H
#define SCENE_H
#include "GameObject.h"
#include <memory>
#include <vector>

namespace DAE
{
    class Scene final
    {
    public:
        void Add(std::unique_ptr<GameObject> object);
        void Remove(GameObject const& object);
        void RemoveAll();

        void Update();
        void Render() const;

        ~Scene() = default;
        Scene(Scene const& other) = delete;
        Scene(Scene&& other) = delete;
        Scene& operator=(Scene const& other) = delete;
        Scene& operator=(Scene&& other) = delete;

    private:
        friend class SceneManager;
        explicit Scene() = default;

        std::vector <std::unique_ptr<GameObject>> m_objects{};
    };

}

#endif
