#pragma once

#include "Matrix.hpp"
#include "Shapes/Shape.hpp"
#include "Tuples/Color.hpp"
#include "Tuples/Point.hpp"

namespace Karbon
{
    struct Shape;

    struct Checker;
    struct Gradient;
    struct Ring;
    struct Stripe;

    struct Pattern
    {
        [[nodiscard]] constexpr Pattern(){};

        // init pattern with first and second color
        [[nodiscard]] constexpr Pattern(Color &first, Color &second)
        {
            m_first_color = first;
            m_second_color = second;
        }

        // init pattern with first and second color and transform matrix
        [[nodiscard]] constexpr Pattern(Color &first, Color &second, Karbon::Matrix4 &transform)
        {
            m_first_color = first;
            m_second_color = second;
            m_transform = transform;
        }

        [[nodiscard]] virtual Karbon::Color color_at(const Karbon::Point &p) const = 0;

        [[nodiscard]] Karbon::Color colot_at(const Shape &s, const Karbon::Point &p) const;

        // == operator
        friend bool operator==(const Pattern &lhs, const Pattern &rhs)
        {
            return lhs.m_first_color == rhs.m_first_color && lhs.m_second_color == rhs.m_second_color &&
                   lhs.m_transform == rhs.m_transform;
        }

        // << operator
        friend std::ostream &operator<<(std::ostream &os, const Pattern &pattern)
        {
            os << "Pattern(" << pattern.m_first_color << ", " << pattern.m_second_color << ")";
            return os;
        }

        // serialize all data to a nlohmann json string object
        [[nodiscard]] virtual std::string to_json() const noexcept = 0;

        Karbon::Matrix4 m_transform = Karbon::IDENTITY;
        Karbon::Color m_first_color = Karbon::WHITE;
        Karbon::Color m_second_color = Karbon::BLACK;
    };

} // namespace Karbon
