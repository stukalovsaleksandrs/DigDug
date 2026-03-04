#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/vec2.hpp>

namespace DAE
{
    class Transform final
    {
    public:
        [[nodiscard]] glm::vec2 GetLocation() const { return m_location; }
        void SetLocation(glm::vec2 const location) { m_location = location; };

    private:
        glm::vec2 m_location{};

    };
}

#endif
