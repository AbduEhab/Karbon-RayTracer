#pragma once

#include <Constants.hpp>
#include <Intersection.hpp>
#include <Materials/Material.hpp>
#include <Matrix.hpp>
#include <Ray.hpp>
#include <Shapes/Shape.hpp>
#include <Tuples/Point.hpp>
#include <Tuples/Vector.hpp>

#include "Patterns/Checker.hpp"
#include "Patterns/Gradient.hpp"
#include "Patterns/Ring.hpp"
#include "Patterns/Stripe.hpp"

namespace Karbon
{
    struct Sphere : public Shape
    {

        [[nodiscard]] Sphere() = default;

        [[nodiscard]] std::pair<float, Shape *> intersects(const Ray &ray) const
        {
            PROFILE_FUNCTION();

            Ray transformed_ray = ray.transform(get_inverse_transform());

            Vector sphere_to_ray = transformed_ray.m_origin - Point();

            float a = transformed_ray.m_direction.dot(transformed_ray.m_direction);
            float b = 2 * sphere_to_ray.dot(transformed_ray.m_direction);
            float c = sphere_to_ray.dot(sphere_to_ray) - 1;

            float discriminant = b * b - 4 * a * c;

            if (discriminant < 0)
            {
                return {};
            }

            float t1 = (-b - std::sqrt(discriminant)) / (2 * a);
            float t2 = (-b + std::sqrt(discriminant)) / (2 * a);

            if (t1 < 0)
            {
                t1 = t2;
            }

            if (t1 < 0)
            {
                return {};
            }

            return {t1, (Shape *)this};
        }

        [[nodiscard]] Vector normal_at(const Point &p) const override
        {
            PROFILE_FUNCTION();

            Point object_point = get_inverse_transform() * p;
            Vector object_normal = (object_point - Point()).normalize();
            return (get_normal_transform() * object_normal).normalize();
        }

        // implement abstract equality
        [[nodiscard]] bool operator==(const Shape &other) const override
        {
            const auto other_sphere = dynamic_cast<const Sphere *>(&other);
            return other_sphere != nullptr && other_sphere->get_transform() == get_transform();
        }

        // get name
        [[nodiscard]] const char *get_name() const override
        {
            return "Sphere ";
        }

        // serialize all data to a nlohmann json string object
        [[nodiscard]] std::string to_json() const noexcept
        {
            nlohmann::json j;

            j["type"] = "Sphere";
            j["translation"] = nlohmann::json::parse(get_translation().to_json());
            j["scale"] = nlohmann::json::parse(get_scale().to_json());
            j["rotation"] = nlohmann::json::parse(get_rotations().to_json());
            j["material"] = nlohmann::json::parse(get_material()->to_json());
            // j["pattern"] = nlohmann::json::parse(get_pattern()->to_json());

            return j.dump();
        }

        // static deserialize all data from a nlohmann json string object
        static std::shared_ptr<Sphere> from_json(const std::string &json) noexcept
        {
            nlohmann::json j = nlohmann::json::parse(json);

            auto sphere = std::make_shared<Sphere>();

            Point translation = Point::from_json(j["translation"].dump());
            Point scale = Point::from_json(j["scale"].dump());
            Point rotation = Point::from_json(j["rotation"].dump());

            float translationf[3] = {translation.x, translation.y, translation.z};
            float scalef[3] = {scale.x, scale.y, scale.z};
            float rotationf[3] = {rotation.x, rotation.y, rotation.z};

            sphere->transform(translationf, rotationf, scalef);

            if (j["material"]["type"] == "Metal")
                sphere->set_material(Metal::from_json(j["material"].dump()));
            else if (j["material"]["type"] == "Lambertian")
                sphere->set_material(Lambertian::from_json(j["material"].dump()));
            else if (j["material"]["type"] == "Dielectric")
                sphere->set_material(Dielectric::from_json(j["material"].dump()));

            // if (j["pattern"]["type"] == "Checker")
            //     sphere->set_pattern(Checker::from_json(j["pattern"].dump()));
            // else if (j["pattern"]["type"] == "Gradient")
            //     sphere->set_pattern(Gradient::from_json(j["pattern"].dump()));
            // else if (j["pattern"]["type"] == "Ring")
            //     sphere->set_pattern(Ring::from_json(j["pattern"].dump()));
            // else if (j["pattern"]["type"] == "Stripe")
            //     sphere->set_pattern(Stripe::from_json(j["pattern"].dump()));

            return sphere;
        }
    };

}; // namespace Karbon
