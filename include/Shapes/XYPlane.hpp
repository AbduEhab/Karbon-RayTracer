#pragma once

#include <Constants.hpp>
#include <Intersection.hpp>
#include <Materials/Material.hpp>
#include <Ray.hpp>
#include <Tuples/Point.hpp>
#include <Tuples/Vector.hpp>

namespace Karbon
{
    struct XYPlane : public Shape
    {
        [[nodiscard]] XYPlane() = default;

        [[nodiscard]] std::pair<float, Shape*> intersects(const Ray &ray) const
        {

            PROFILE_FUNCTION();

            Ray transformed_ray = ray.transform(get_inverse_transform());

            if (std::abs(transformed_ray.m_direction.z) < kEpsilon)
            {
                return {};
            }

            float t = -(transformed_ray.m_origin.z) / (transformed_ray.m_direction.z);

            if (t < 0)
            {
                return {};
            }

            return {t, (Shape *)this};
        }

        [[nodiscard]] Vector normal_at([[maybe_unused]] const Point &p) const override
        {
            PROFILE_FUNCTION();

            return Vector(0, 0, 1);
        }

        // implement abstract equality
        [[nodiscard]] bool operator==(const Shape &other) const override
        {
            const auto other_XY_plane = dynamic_cast<const XYPlane *>(&other);
            return other_XY_plane != nullptr && other_XY_plane->get_transform() == get_transform();
        };

        // get name
        [[nodiscard]] const char *get_name() const override
        {
            return "XYPlane ";
        }

        // serialize all data to a nlohmann json string object
        [[nodiscard]] std::string to_json() const noexcept
        {
            nlohmann::json j;

            j["type"] = "XYPlane";
            j["translation"] = nlohmann::json::parse(get_translation().to_json());
            j["scale"] = nlohmann::json::parse(get_scale().to_json());
            j["rotation"] = nlohmann::json::parse(get_rotations().to_json());
            // j["material"] = nlohmann::json::parse(get_material()->to_json());

            return j.dump(4);
        }

        // static deserialize from a nlohmann json string object
        static std::shared_ptr<XYPlane> from_json(const std::string &json) noexcept
        {
            nlohmann::json j = nlohmann::json::parse(json);

            auto XY_plane = std::make_shared<XYPlane>();

            Point translation = Point::from_json(j["translation"].dump());
            Point scale = Point::from_json(j["scale"].dump());
            Point rotation = Point::from_json(j["rotation"].dump());

            float translationf[3] = {translation.x, translation.y, translation.z};
            float scalef[3] = {scale.x, scale.y, scale.z};
            float rotationf[3] = {rotation.x, rotation.y, rotation.z};

            XY_plane->transform(translationf, rotationf, scalef);

            if (j["material"]["type"] == "Metal")
                XY_plane->set_material(Metal::from_json(j["material"].dump()));
            else if (j["material"]["type"] == "Lambertian")
                XY_plane->set_material(Lambertian::from_json(j["material"].dump()));

            return XY_plane;
        }
    };
}; // namespace Karbon
