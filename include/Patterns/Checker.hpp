#pragma once

#include "Shapes/Shape.hpp"
#include "Tuples/Color.hpp"
#include "Tuples/Point.hpp"

#include "Patterns/Pattern.hpp"

namespace Karbon
{
    struct Checker : public Pattern
    {
        [[nodiscard]] constexpr Checker() : Pattern() {}

        [[nodiscard]] constexpr Checker(Karbon::Color &first, Karbon::Color &second) : Pattern(first, second) {}

        [[nodiscard]] constexpr Checker(Karbon::Color &first, Karbon::Color &second, Karbon::Matrix4 &transform) : Pattern(first, second, transform) {}

        [[nodiscard]] Karbon::Color color_at(const Karbon::Point &p) const override
        {
            if ((int)(std::abs(p.x) + std::abs(p.y) + std::abs(p.z)) % 2 == 0)
                return m_first_color;
            else
                return m_second_color;
        }

        // serialize all data to a nlohmann json string object
        [[nodiscard]] std::string to_json() const noexcept
        {
            nlohmann::json json;
            json["type"] = "Checker";
            json["first_color"] = nlohmann::json::parse(m_first_color.to_json());
            json["second_color"] = nlohmann::json::parse(m_second_color.to_json());
            json["transform"] = nlohmann::json::parse(m_transform.to_json());
            return json.dump();
        }

        // static deserialize all data from a nlohmann json string object
        static std::shared_ptr<Pattern> from_json(const std::string &json)
        {
            nlohmann::json json_object = nlohmann::json::parse(json);

            auto first_color = Color::from_json(json_object["first_color"].dump());
            auto second_color = Color::from_json(json_object["second_color"].dump());
            auto transform = Matrix4::from_json(json_object["transform"].dump());
            return std::make_shared<Checker>(first_color, second_color, transform);
        }
    };
} // namespace Karbon
