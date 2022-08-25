#pragma once

#include "Shapes/Shape.hpp"
#include "Tuples/Point.hpp"
#include "Tuples/Vector.hpp"

struct Shape;

namespace COAL
{
    struct Computation
    {

        [[nodiscard]] constexpr Computation(const float t, const Shape &s, const Point &p, const Vector &eye_vector, const Vector &normal_vector,
                                            const bool inside, const Point &over_point, const Vector &reflection_vector, const float n1, const float n2, const Point &under_point)
            : m_t(t), m_s(&s), m_p(p), m_eye_vector(eye_vector), m_normal_vector(normal_vector), m_inside(inside), m_over_point(over_point),
              m_reflection_vector(reflection_vector), m_n1(n1), m_n2(n2), m_under_point(under_point)
        {
        }

        [[nodiscard]] constexpr float schilck() const noexcept
        {
            float cosine = m_eye_vector.dot(m_normal_vector);

            if (m_n1 > m_n2)
            {
                float n = m_n1 / m_n2;
                float sine2_t = n * n * (1 - cosine * cosine);

                if (sine2_t > 1)
                {
                    return 1;
                }

                float cos_t = sqrt(1 - sine2_t);

                cosine = cos_t;
            }

            float r0 = ((m_n1 - m_n2) / (m_n1 + m_n2));
            r0 = r0 * r0;

            return r0 + (1.0f - r0) * (float)std::pow((1 - cosine), 5);
        }

        float m_t;
        const Shape *m_s;
        Point m_p;
        Vector m_eye_vector;
        Vector m_normal_vector;
        bool m_inside;
        Point m_over_point;
        Vector m_reflection_vector;
        float m_n1;
        float m_n2;
        Point m_under_point;
    };
} // namespace COAL
