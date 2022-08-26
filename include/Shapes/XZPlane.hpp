#pragma once

#include <Constants.hpp>
#include <Intersection.hpp>
#include <Material.hpp>
#include <Ray.hpp>
#include <Tuples/Point.hpp>
#include <Tuples/Vector.hpp>

namespace Karbon
{
    struct XZPlane : public Shape
    {
        [[nodiscard]] XZPlane() = default;

        [[nodiscard]] Intersection intersects(const Ray &ray) const
        {

            PROFILE_FUNCTION();

            Ray transformed_ray = ray.transform(get_inverse_transform());

            if (std::abs(transformed_ray.m_direction.y) < kEpsilon)
            {
                return {};
            }

            float t = -(transformed_ray.m_origin.y) / (transformed_ray.m_direction.y);

            if (t < 0)
            {
                return {};
            }

            return Intersection(t, *this);
        }

        [[nodiscard]] Vector normal_at([[maybe_unused]] const Point &p) const override
        {
            PROFILE_FUNCTION();

            return Vector(0, 1, 0);
        }

        // implement abstract equality
        [[nodiscard]] bool operator==(const Shape &other) const override
        {
            const auto other_XZ_plane = dynamic_cast<const XZPlane *>(&other);
            return other_XZ_plane != nullptr && other_XZ_plane->get_transform() == get_transform();
        };

        // get name
        [[nodiscard]] const char *get_name() const override
        {
            return "XZPlane ";
        }

        // serialize all data to a nlohmann json string object
        [[nodiscard]] std::string to_json() const noexcept
        {
            nlohmann::json j;

            j["type"] = "XZPlane";
            j["translation"] = nlohmann::json::parse(get_translation().to_json());
            j["scale"] = nlohmann::json::parse(get_scale().to_json());
            j["rotation"] = nlohmann::json::parse(get_rotations().to_json());
            j["material"] = nlohmann::json::parse(get_material().to_json());

            return j.dump(4);
        }

        // static deserialize from a nlohmann json string object
        static std::shared_ptr<XZPlane> from_json(const std::string &json) noexcept
        {
            nlohmann::json j = nlohmann::json::parse(json);

            auto XZ_plane = std::make_shared<XZPlane>();

            Point translation = Point::from_json(j["translation"].dump());
            Point scale = Point::from_json(j["scale"].dump());
            Point rotation = Point::from_json(j["rotation"].dump());

            float translationf[3] = {translation.x, translation.y, translation.z};
            float scalef[3] = {scale.x, scale.y, scale.z};
            float rotationf[3] = {rotation.x, rotation.y, rotation.z};

            XZ_plane->transform(translationf, rotationf, scalef);

            XZ_plane->set_material(Material::from_json(j["material"].dump()));

            return XZ_plane;
        }
    };
}; // namespace Karbon
