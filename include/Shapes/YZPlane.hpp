#pragma once

#include <Constants.hpp>
#include <Intersection.hpp>
#include <Material.hpp>
#include <Ray.hpp>
#include <Tuples/Point.hpp>
#include <Tuples/Vector.hpp>

namespace COAL
{
    struct YZPlane : public Shape
    {
        [[nodiscard]] YZPlane() = default;

        [[nodiscard]] Intersection intersects(const Ray &ray) const
        {

            PROFILE_FUNCTION();

            Ray transformed_ray = ray.transform(get_inverse_transform());

            if (std::abs(transformed_ray.m_direction.x) < kEpsilon)
            {
                return {};
            }

            float t = -(transformed_ray.m_origin.x) / (transformed_ray.m_direction.x);

            if (t < 0)
            {
                return {};
            }

            return Intersection(t, *this);
        }

        [[nodiscard]] Vector normal_at([[maybe_unused]] const Point &p) const override
        {
            PROFILE_FUNCTION();

            return Vector(1, 0, 0);
        }

        // implement abstract equality
        [[nodiscard]] bool operator==(const Shape &other) const override
        {
            const auto other_YZ_plane = dynamic_cast<const YZPlane *>(&other);
            return other_YZ_plane != nullptr && other_YZ_plane->get_transform() == get_transform();
        };

        // get name
        [[nodiscard]] const char *get_name() const override
        {
            return "YZPlane ";
        }

        // serialize all data to a nlohmann json string object
        [[nodiscard]] std::string to_json() const noexcept
        {
            nlohmann::json j;

            j["type"] = "YZPlane";
            j["translation"] = nlohmann::json::parse(get_translation().to_json());
            j["scale"] = nlohmann::json::parse(get_scale().to_json());
            j["rotation"] = nlohmann::json::parse(get_rotations().to_json());
            j["material"] = nlohmann::json::parse(get_material().to_json());

            return j.dump(4);
        }

        // static deserialize from a nlohmann json string object
        static std::shared_ptr<YZPlane> from_json(const std::string &json) noexcept
        {
            nlohmann::json j = nlohmann::json::parse(json);

            auto YZ_plane = std::make_shared<YZPlane>();

            Point translation = Point::from_json(j["translation"].dump());
            Point scale = Point::from_json(j["scale"].dump());
            Point rotation = Point::from_json(j["rotation"].dump());

            float translationf[3] = {translation.x, translation.y, translation.z};
            float scalef[3] = {scale.x, scale.y, scale.z};
            float rotationf[3] = {rotation.x, rotation.y, rotation.z};

            YZ_plane->transform(translationf, rotationf, scalef);

            YZ_plane->set_material(Material::from_json(j["material"].dump()));

            return YZ_plane;
        }
    };
}; // namespace COAL
