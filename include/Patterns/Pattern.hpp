#pragma once

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
        [[nodiscard]] constexpr Pattern(Color &first, Color &second);

        // init pattern with first and second color and transform matrix
        [[nodiscard]] constexpr Pattern(Color &first, Color &second, Karbon::Matrix4 &transform);

        [[nodiscard]] virtual Karbon::Color color_at([[maybe_unused]] const Karbon::Point &p) const
        {
            return Karbon::BLACK;
        }

        [[nodiscard]] Karbon::Color color_at(Shape &s, Karbon::Point &p) const;

        // serialize all data to a nlohmann json string object
        [[nodiscard]] virtual std::string to_json() const noexcept = 0;

        Karbon::Matrix4 m_transform;
        Karbon::Matrix4 m_inverse_transform;
        Karbon::Color m_first_color;
        Karbon::Color m_second_color;
    };

} // namespace Karbon
