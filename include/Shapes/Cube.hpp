#pragma once

#include <Constants.hpp>
#include <Intersection.hpp>
#include <Material.hpp>
#include <Matrix.hpp>
#include <Ray.hpp>
#include <Shapes/Shape.hpp>
#include <Tuples/Point.hpp>
#include <Tuples/Vector.hpp>

namespace Karbon
{
    struct Cube : public Shape
    {

        [[nodiscard]] Cube() = default;

        [[nodiscard]] Intersection intersects(const Ray &ray) const
        {
            PROFILE_FUNCTION();

            auto check_axis = [](const float origin, const float direction)
            {
                float tmin_numerator = -1 - origin;
                float tmax_numerator = 1 - origin;

                float tmin;
                float tmax;

                if (std::abs(direction) >= kEpsilon)
                {
                    tmin = tmin_numerator / direction;
                    tmax = tmax_numerator / direction;
                }
                else
                {
                    tmin = tmin_numerator * std::numeric_limits<float>::infinity();
                    tmax = tmax_numerator * std::numeric_limits<float>::infinity();
                }

                if (tmin > tmax)
                    std::swap(tmin, tmax);

                return std::make_pair(tmin, tmax);
            };

            auto transformed_ray = ray.transform(get_inverse_transform());

            auto xt = check_axis(transformed_ray.m_origin.x, transformed_ray.m_direction.x);
            auto yt = check_axis(transformed_ray.m_origin.y, transformed_ray.m_direction.y);
            auto zt = check_axis(transformed_ray.m_origin.z, transformed_ray.m_direction.z);

            float tmin = std::max(xt.first, std::max(yt.first, zt.first));
            float tmax = std::min(xt.second, std::min(yt.second, zt.second));

            if (tmin > tmax)
                return {};

            return {tmin, *this};
        }

        [[nodiscard]] Vector normal_at(const Point &p) const override
        {
            PROFILE_FUNCTION();

            auto local_p = get_inverse_transform() * p;

            float maxP = std::max(std::abs(local_p.x), std::max(std::abs(local_p.y), std::abs(local_p.z)));

            if (maxP == std::abs(local_p.x))
                return Vector(local_p.x, 0, 0);

            if (maxP == std::abs(local_p.y))
                return Vector(0, local_p.y, 0);

            return Vector(0, 0, local_p.z);
        }

        // implement abstract equality
        [[nodiscard]] bool operator==(const Shape &other) const override
        {
            const auto other_sphere = dynamic_cast<const Cube *>(&other);
            return other_sphere != nullptr && other_sphere->get_transform() == get_transform();
        }

        // get name
        [[nodiscard]] const char *get_name() const override
        {
            return "Cube ";
        }

        // serialize all data to a nlohmann json string object
        [[nodiscard]] std::string to_json() const noexcept
        {
            nlohmann::json j;

            j["type"] = "Cube";
            j["translation"] = nlohmann::json::parse(get_translation().to_json());
            j["scale"] = nlohmann::json::parse(get_scale().to_json());
            j["rotation"] = nlohmann::json::parse(get_rotations().to_json());
            j["material"] = nlohmann::json::parse(get_material().to_json());

            return j.dump();
        }

        // static deserialize all data from a nlohmann json string object
        static std::shared_ptr<Cube> from_json(const std::string &json) noexcept
        {
            nlohmann::json j = nlohmann::json::parse(json);

            auto cube = std::make_shared<Cube>();

            Point translation = Point::from_json(j["translation"].dump());
            Point scale = Point::from_json(j["scale"].dump());
            Point rotation = Point::from_json(j["rotation"].dump());

            float translationf[3] = {translation.x, translation.y, translation.z};
            float scalef[3] = {scale.x, scale.y, scale.z};
            float rotationf[3] = {rotation.x, rotation.y, rotation.z};

            cube->transform(translationf, rotationf, scalef);

            cube->set_material(Material::from_json(j["material"].dump()));

            return cube;
        }
    };
}; // namespace Karbon
