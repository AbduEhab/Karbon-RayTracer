#pragma once

#include "Materials/Material.hpp"
#include "Ray.hpp"
#include "Tuples/Color.hpp"

#include "json.hpp"

#include "Computation.hpp"

namespace Karbon
{

    struct Dielectric : public Material
    {

        [[nodiscard]] constexpr Dielectric(const Color &c = Color(1.0, 1.0, 1.0), const float refractive_index = 1.05f)
        {
            set_color(c);
            set_refractive_index(refractive_index);
        }

        [[nodiscard]] constexpr Dielectric(const Dielectric &other)
        {
            set_color(other.get_color());
            set_refractive_index(other.get_refractive_index());
        }

        bool scatter(const Ray &r_in, const Computation &comp, Color &attenuation, Ray &scattered) const
        {
            attenuation = Color(1.0, 1.0, 1.0);

            float n_ratio = comp.m_inside ? comp.m_n1 / comp.m_n2 : comp.m_n2 / comp.m_n1;

            float cos_i = comp.m_eye_vector.dot(comp.m_normal_vector);

            float sin2_t = (n_ratio * n_ratio * (1.0f - cos_i * cos_i));

            Vector direction;

            if (sin2_t > 1.0 || comp.schilck() > random<float>())
                direction = comp.m_reflection_vector;
            else
            {
                float cos_t = (float)sqrt(1.0 - sin2_t);

                direction = comp.m_normal_vector * (n_ratio * cos_i - cos_t) - comp.m_eye_vector * n_ratio;
            }

            scattered = Ray(comp.m_under_point, direction);

            return true;
        }

        [[nodiscard]] virtual const char *get_name() const
        {
            return "Dielectric";
        }

        // serialize all data to a nlohmann json string object
        [[nodiscard]] virtual nlohmann::json to_json() const
        {
            nlohmann::json j;
            j["type"] = "Dielectric";
            j["color"] = nlohmann::json::parse(get_color().to_json());
            return j.dump();
        }

        // deserialize all data from a nlohmann json string object
        [[nodiscard]] static std::shared_ptr<Dielectric> from_json(const nlohmann::json &json)
        {
            // nlohmann::json j = nlohmann::json::parse(json);

            auto mat = std::make_shared<Dielectric>();

            // mat->set_color(Color::from_json(j["color"].dump()));
            // mat->set_refractive_index(j["refractive_index"]);

            return mat;
        }
    };
};