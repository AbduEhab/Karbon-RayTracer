#pragma once

#include "Computation.hpp"
#include "Constants.hpp"
#include "Ray.hpp"
#include "Shapes/Shape.hpp"
#include "Tuples/Point.hpp"
#include "Tuples/Vector.hpp"

namespace Karbon
{
    struct Intersection
    {

        [[nodiscard]] constexpr Intersection() : m_t(-1), m_object(nullptr) {}

        [[nodiscard]] constexpr Intersection(const float t, const Shape &object)
            : m_t(t), m_object(&object)
        {
        }

        [[nodiscard]] Computation prepare_computation(const Ray &ray, const std::vector<Intersection> &xs) const
        {
            PROFILE_FUNCTION();

            float t2 = m_t;
            const Shape *object = m_object;
            Point p = ray.position(m_t);
            Vector eyev = -ray.m_direction;
            Vector normalv = object->normal_at(p);
            Vector reflectv = ray.m_direction.reflect(normalv);

            bool inside = false;

            if (normalv.dot(eyev) < 0)
            {
                normalv = -normalv;
                inside = true;
            }

            Point over_point = p + normalv * 1e-4f;
            Point under_point = p - normalv * 1e-4f;

            float n1 = 0;
            float n2 = 0;

            if (!xs.empty())
            {
                std::deque<const Karbon::Shape *> shape_deque;

                for (const Karbon::Intersection &intersection : xs)
                {
                    if (intersection == *this)
                        if (shape_deque.empty())
                            n1 = 1;
                        else
                            n1 = shape_deque.back()->get_material().get_refractive_index();

                    if (contains(shape_deque, *intersection.m_object))
                        remove(shape_deque, *intersection.m_object);
                    else
                        shape_deque.emplace_back(intersection.m_object);

                    if (intersection == *this)
                    {
                        if (shape_deque.empty())
                            n2 = 1;
                        else
                            n2 = shape_deque.back()->get_material().get_refractive_index();
                        break;
                    }
                }
            }

            return Computation(t2, *object, p, eyev, normalv, inside, over_point, reflectv, n1, n2, under_point);
        }

        [[nodiscard]] static Intersection hit(std::vector<Intersection> intersections)
        {
            PROFILE_FUNCTION();

            for (const Intersection &intersection : intersections)
            {
                if (intersection.m_t > 0)
                    return intersection;
            }

            return {};
        }

        // == operator
        [[nodiscard]] constexpr bool operator==(const Intersection &other) const noexcept
        {
            return m_t == other.m_t && m_object == other.m_object;
        }

        // == operator
        [[nodiscard]] constexpr bool operator==(Intersection &other) const noexcept
        {
            return m_t == other.m_t && m_object == other.m_object;
        }

    private:
        // check if deque containts a certain intersction
        [[nodiscard]] bool contains(const std::deque<const Karbon::Shape *> &deque, const Karbon::Shape &element) const
        {
            for (const auto &shape : deque)
            {
                if (*shape == element)
                    return true;
            }

            return false;
        }

        // remove a shape from deque
        [[nodiscard]] void remove(std::deque<const Karbon::Shape *> &deque, const Karbon::Shape &element) const
        {
            for (auto it = deque.begin(); it != deque.end(); it++)
            {
                if (*it == &element)
                {
                    deque.erase(it);
                    break;
                }
            }
        }

    public:
        float m_t;
        const Shape *m_object;
    };

    struct intersection_return_type
    {

        Intersection i1;
        Intersection i2;
    };
} // namespace Karbon
