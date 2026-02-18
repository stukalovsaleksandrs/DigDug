#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/vec3.hpp>

namespace DAE
{
    class Transform final
    {
    public:
        glm::vec3 const& GetLocation() const { return m_location; }
        void SetLocation(glm::vec3 const& location);

    private:
        glm::vec3 m_location{};

    };
}

#endif
