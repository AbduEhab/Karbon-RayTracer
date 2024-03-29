#pragma once

#include "Shapes/Shape.hpp"
#include "Tuples/Color.hpp"
#include "Tuples/Point.hpp"

#include "Patterns/Pattern.hpp"

namespace Karbon
{
    struct Stripe : public Pattern
    {
        [[nodiscard]] constexpr Stripe() : Pattern() {}

        [[nodiscard]] constexpr Stripe(Karbon::Color &first, Karbon::Color &second) : Pattern(first, second) {}

        [[nodiscard]] constexpr Stripe(Karbon::Color &first, Karbon::Color &second, Karbon::Matrix4 &transform) : Pattern(first, second, transform) {}

        [[nodiscard]] Karbon::Color color_at(const Karbon::Point &p) const override
        {
            if ((int)(std::floor(p.x)) % 2 == 0)
                return m_first_color;
            else
                return m_second_color;
        }

        // serialize all data to a nlohmann json string object
        [[nodiscard]] std::string to_json() const noexcept
        {
            nlohmann::json json;
            json["type"] = "Stripe";
            json["first_color"] = nlohmann::json::parse(m_first_color.to_json());
            json["second_color"] = nlohmann::json::parse(m_second_color.to_json());
            json["transform"] = nlohmann::json::parse(m_transform.to_json());
            return json.dump();
        }

        // static deserialize all data from a nlohmann json string object
        static std::shared_ptr<Pattern> from_json(const nlohmann::json &json)
        {
            auto first_color = Color::from_json(json["first_color"]);
            auto second_color = Color::from_json(json["second_color"]);
            auto transform = Matrix4::from_json(json["transform"]);
            return std::make_shared<Stripe>(first_color, second_color, transform);
        }
    };

} // namespace Karbon
