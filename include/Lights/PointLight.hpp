#pragma once

#include "../Constants.hpp"
#include "./Tuples/Color.hpp"
#include "./Tuples/Vector.hpp"
#include "json.hpp"

namespace COAL
{
    struct PointLight : public Light
    {
        [[nodiscard]] constexpr PointLight() : Light() {}

        [[nodiscard]] constexpr PointLight(COAL::Point &position, COAL::Color &intensity) : Light(position, intensity) {}

        // implement abstract equality
        [[nodiscard]] bool operator==(const Light &rhs) const noexcept override
        {
            const auto other_point_light = dynamic_cast<const PointLight *>(&rhs);
            return other_point_light != nullptr && other_point_light->get_position() == get_position() && other_point_light->get_intensity() == get_intensity();
        }

        // get name
        [[nodiscard]] const char *get_name() const override
        {
            return "PointLight ";
        }

        // serialize all data to a nlohmann json string object
        [[nodiscard]] std::string to_json() const noexcept
        {
            nlohmann::json json;
            json["type"] = "PointLight";
            json["position"] = nlohmann::json::parse(get_position().to_json());
            json["intensity"] = nlohmann::json::parse(get_intensity().to_json());
            return json.dump();
        }

        // static deserialize from a nlohmann json string object
        static std::shared_ptr<PointLight> from_json(const std::string &json)
        {
            nlohmann::json json_object = nlohmann::json::parse(json);

            auto PointLight = std::make_shared<COAL::PointLight>(COAL::PointLight());

            PointLight->set_position(COAL::Point::from_json(json_object["position"].dump()));
            PointLight->set_intensity(COAL::Color::from_json(json_object["intensity"].dump()));

            return PointLight;
        }
    };

} // namespace COAL
