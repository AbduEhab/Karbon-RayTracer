#pragma once

#include "Shapes/Shape.hpp"
#include "Tuples/Color.hpp"
#include "Tuples/Point.hpp"

namespace COAL
{
    struct Gradient : public Pattern
    {
        [[nodiscard]] constexpr Gradient() : Pattern() {}

        [[nodiscard]] constexpr Gradient(COAL::Color &first, COAL::Color &second) : Pattern(first, second) {}

        [[nodiscard]] constexpr Gradient(COAL::Color &first, COAL::Color &second, COAL::Matrix4 &transform) : Pattern(first, second, transform) {}

        [[nodiscard]] COAL::Color color_at(const COAL::Point &p) const override
        {
            // float x = p.x;
            // float y = p.y;
            // float z = p.z;

            // float r = (x * m_transform[0][0] + y * m_transform[0][1] + z * m_transform[0][2] + m_transform[0][3]) /
            //            (x * m_transform[3][0] + y * m_transform[3][1] + z * m_transform[3][2] + m_transform[3][3]);
            // float g = (x * m_transform[1][0] + y * m_transform[1][1] + z * m_transform[1][2] + m_transform[1][3]) /
            //            (x * m_transform[3][0] + y * m_transform[3][1] + z * m_transform[3][2] + m_transform[3][3]);
            // float b = (x * m_transform[2][0] + y * m_transform[2][1] + z * m_transform[2][2] + m_transform[2][3]) /
            //            (x * m_transform[3][0] + y * m_transform[3][1] + z * m_transform[3][2] + m_transform[3][3]);

            // return COAL::Color(r, g, b);

            Color distance = m_second_color - m_first_color;
            float fraction = p.x - std::floor(p.x);

            return m_first_color + distance * fraction;
        }

        // // == operator
        // friend bool operator==(const Pattern &lhs, const Pattern &rhs)
        // {
        //     const auto other_gradient = dynamic_cast<const Gradient *>(&rhs);
        // }

        // serialize all data to a nlohmann json string object
        [[nodiscard]] std::string to_json() const noexcept
        {
            nlohmann::json json;
            json["type"] = "Gradient";
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
            return std::make_shared<Gradient>(first_color, second_color, transform);
        }
    };
} // namespace COAL
