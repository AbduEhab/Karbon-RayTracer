#pragma once

#include "Materials/Material.hpp"
#include "Ray.hpp"
#include "Tuples/Color.hpp"

#include "Computation.hpp"

namespace Karbon
{

    struct Metal : public Material
    {

        [[nodiscard]] constexpr Metal(const Color &c = Color(1.0, 1.0, 1.0))
        {
            set_color(c);
        }

        bool scatter(const Ray &r_in, const Computation &comp, Color &attenuation, Ray &scattered) const
        {
            scattered = Ray(comp.m_over_point, comp.m_reflection_vector);
            attenuation = get_color();
            return (scattered.m_direction.dot(comp.m_normal_vector) > 0);
        }

        [[nodiscard]] virtual const char *get_name() const
        {
            return "Metal";
        }

        // serialize all data to a nlohmann json string object
        [[nodiscard]] virtual nlohmann::json to_json() const
        {
            nlohmann::json j;
            j["type"] = "Metal";
            j["color"] = nlohmann::json::parse(get_color().to_json());
            j["refractive_index"] = get_refractive_index();
            return j.dump();
        }

        // deserialize all data from a nlohmann json string object
        [[nodiscard]] static std::shared_ptr<Metal> from_json(const nlohmann::json &json)
        {
            // nlohmann::json j = nlohmann::json::parse(json);

            auto mat = std::make_shared<Metal>();

            // mat->set_color(Color::from_json(j["color"].dump()));
            // mat->set_refractive_index(j["refractive_index"]);

            return mat;
        }
    };
};