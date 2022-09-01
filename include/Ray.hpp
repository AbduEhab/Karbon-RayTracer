#pragma once

#include "Tuples/Point.hpp"
#include "Tuples/Vector.hpp"

namespace Karbon
{
    struct Ray
    {
    public:
        [[nodiscard]] constexpr Ray()
        {
        }

        [[nodiscard]] constexpr Ray(const Point &origin, const Vector &direction)
            : m_origin(origin), m_direction(direction)
        {
        }

        [[nodiscard]] static Ray random_in_unit_sphere(const Point &p)
        {
            return Ray(p, Vector::random_in_unit_sphere());
        }

        [[nodiscard]] static Ray random_in_unit_sphere_with_direction(const Point &p, const Vector &direction)
        {
            Vector res = Vector::random_in_unit_sphere();

            while (res.dot(direction) <= 0)
            {
                res = Vector::random_in_unit_sphere();
            }

            return Ray(p, res);
        }

        [[nodiscard]] Point position(const float t) const
        {
            return m_origin + m_direction * t;
        }

        [[nodiscard]] Ray transform(const Matrix4 &matrix) const
        {
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
} // namespace Karbon
