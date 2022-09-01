#pragma once

#include "Materials/Material.hpp"
#include "Ray.hpp"
#include "Tuples/Color.hpp"

#include "json.hpp"

#include "Computation.hpp"

namespace Karbon
{

    struct Lambertian : public Material
    {

        [[nodiscard]] constexpr Lambertian(const Color &c = Color(1.0, 1.0, 1.0))
        {
            set_color(c);
        }

        [[nodiscard]] constexpr Lambertian(const Lambertian &other)
        {
            set_color(other.get_color());
        }

        bool scatter(const Ray &r_in, const Computation &comp, Color &attenuation, Ray &scattered) const;

        [[nodiscard]] virtual const char *get_name() const
        {
            return "Lambertian";
        }

        // serialize all data to a nlohmann json string object
        [[nodiscard]] virtual nlohmann::json to_json() const
        {
            nlohmann::json j;
            j["type"] = "Lambertian";
            j["color"] = nlohmann::json::parse(get_color().to_json());
            return j.dump();
        }

        // deserialize all data from a nlohmann json string object
        [[nodiscard]] static std::shared_ptr<Lambertian> from_json(const nlohmann::json &json)
        {
            // nlohmann::json j = nlohmann::json::parse(json);

            auto mat = std::make_shared<Lambertian>();

            // mat->set_color(Color::from_json(j["color"].dump()));
            // mat->set_refractive_index(j["refractive_index"]);

            return mat;
        }
    };
};