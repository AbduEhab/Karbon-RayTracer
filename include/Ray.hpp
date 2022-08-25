#pragma once

#include "Tuples/Point.hpp"
#include "Tuples/Vector.hpp"

namespace COAL
{
    struct Ray
    {
    public:
        [[nodiscard]] constexpr Ray(const Point &origin, const Vector &direction)
            : m_origin(origin), m_direction(direction)
        {
        }

        [[nodiscard]] Point position(const float t) const
        {
            PROFILE_FUNCTION();

            return m_origin + m_direction * t;
        }

        [[nodiscard]] Ray transform(const Matrix4 &matrix) const
        {
            PROFILE_FUNCTION();

            Point new_origin = matrix * m_origin;
            Vector new_direction = matrix * m_direction;

            return Ray(new_origin, new_direction);
        }

        // << operator
        friend std::ostream &operator<<(std::ostream &os, const Ray &r)
        {

            os << "Ray(origin=" << r.m_origin << ", direction=" << r.m_direction << ")";
            return os;
        }

        Point m_origin;
        Vector m_direction;
    };
} // namespace COAL
