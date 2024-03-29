#pragma once

#include "Materials/Material.hpp"
#include "Ray.hpp"
#include "Tuples/Color.hpp"

#include "json.hpp"

#include "Computation.hpp"

namespace Karbon
{

    struct Metal : public Material
    {

        [[nodiscard]] constexpr Metal(const Color &c = Color(1.0, 1.0, 1.0), const float roughness = 0.0)
        {
            set_color(c);
            set_roughness(roughness);
        }

        [[nodiscard]] constexpr Metal(const Metal &other)
        {
            set_color(other.get_color());
            m_roughness = other.m_roughness;
        }

        bool scatter(const Computation &comp, Color &attenuation, Ray &scattered) const
        {
            scattered = Ray(comp.m_over_point, comp.m_reflection_vector + m_roughness * Vector::random_in_unit_sphere());

            // attenuation = comp.m_s->get_pattern()->color_at(comp.m_p);
            attenuation = get_color();

            return (scattered.m_direction.dot(comp.m_normal_vector) > 0);
        }

        [[nodiscard]] virtual const char *get_name() const
        {
            return "Metal";
        }

        // serialize all data to a nlohmann json string object
        [[nodiscard]] virtual std::string to_json() const
        {
            nlohmann::json j;
            j["type"] = "Metal";
            j["refractive_index"] = get_refractive_index();
            j["color"] = nlohmann::json::parse(get_color().to_json());
            return j.dump();
        }

        // deserialize all data from a nlohmann json string object
        [[nodiscard]] static std::shared_ptr<Metal> from_json(const std::string &json)
        {
            nlohmann::json j = nlohmann::json::parse(json);

            auto mat = std::make_shared<Metal>();

            mat->set_color(Color::from_json(j["color"].dump()));
            mat->set_refractive_index(j["refractive_index"]);

            return mat;
        }

        // setters and getters
        [[nodiscard]] float get_roughness()
        {
            return m_roughness;
        }

        constexpr Metal &set_roughness(float s)
        {
            m_roughness = s;
            return *this;
        }

    private:
        float m_roughness = 0;
    };
};