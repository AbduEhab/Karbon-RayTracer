#pragma once

#include "Computation.hpp"
#include "Constants.hpp"
#include "Intersection.hpp"
#include "Lights/Light.hpp"
#include "Lights/PointLight.hpp"
#include "Materials/Metal.hpp"
#include "Matrix.hpp"
#include "Shapes/Shape.hpp"
#include "Shapes/Sphere.hpp"
#include "Tuples/Color.hpp"
#include "Tuples/Point.hpp"
#include "Tuples/Vector.hpp"

namespace Karbon
{
    struct World
    {

        [[nodiscard]] World() = default;

        [[nodiscard]] World([[maybe_unused]] const int type)
        {
            PROFILE_FUNCTION();

            auto floor = std::make_shared<XZPlane>(XZPlane());
            floor->get_material()->set_color(Color(1.0f, 0.9f, 0.9f));

            auto middle_sphere = std::make_shared<Sphere>(Sphere());
            middle_sphere->set_material(std::make_shared<Metal>(Metal(Color(1.0f, 1.0f, 1.0f))));
            middle_sphere->translate(-0.5f, 1.0f, 0.5f);

            auto right_sphere = std::make_shared<Sphere>(Sphere());
            right_sphere->get_material()->set_color(Color(0.5f, 1.0f, 0.1f));
            right_sphere->translate(1.5f, 0.5f, -0.5f).scale(0.5f, 0.5f, 0.5f);

            auto left_sphere = std::make_shared<Sphere>(Sphere());
            left_sphere->get_material()->set_color(Color(1, 0.8f, 0.1f));
            left_sphere->translate(-1.5f, 0.33f, -0.75f).scale(0.33f, 0.33f, 0.33f);

            // auto light = std::make_shared<Karbon::PointLight>(Karbon::PointLight());
            // light->set_intensity(Karbon::Color((float)255, (float)255, (float)255));

            add_shapes({floor, middle_sphere, right_sphere, left_sphere});
            // add_lights({light});
        }

        [[nodiscard]] std::vector<std::pair<float, Shape *>> intersects(const Ray &ray) const
        {
            PROFILE_FUNCTION();

            std::vector<std::pair<float, Shape *>> res;

            for (const auto &shape : m_shapes)
            {
                auto shape_xs = shape->intersects(ray);
                if (shape_xs.first > 0)
                    res.emplace_back(shape_xs);
            }

            // lamda std::vector sort
            std::sort(res.begin(), res.end(), [](const std::pair<float, Shape *> &a, const std::pair<float, Shape *> &b)
                      { return a.first < b.first; });

            return res;
        }

        [[nodiscard]] Color color_at(const Ray &ray, const int recurtion_level = 0) const
        {
            auto xs = intersects(ray);
            std::pair<float, Shape *> hit = Intersection::hit(xs);

            // If we've exceeded the ray bounce limit, no more light is gathered.
            if (recurtion_level > max_recurtion_level)
                return Karbon::BLACK;

            if (hit.first > 0)
            {
                Intersection isect = Intersection(hit.first, *hit.second);

                auto comp = isect.prepare_computation(ray, xs);

                Ray scattered;
                Color attenuation;

                if (hit.second->get_material()->scatter(ray, comp, attenuation, scattered))
                    return attenuation * color_at(scattered, recurtion_level + 1);

                return Karbon::BLACK;
            }

            const float t = (float)(0.5 * (float)(ray.m_direction.y + 1.0));
            return (1.0f - t) * Color(255.0, 255.0, 255.0) + t * Color(127.5, 178.5, 255);
        }

        // add shapes
        void add_shape(const std::shared_ptr<Shape> &shape)
        {
            m_shapes.emplace_back(shape);
        }

        // add shapes
        void add_shapes(const std::vector<std::shared_ptr<Shape>> &shapes)
        {
            m_shapes.insert(m_shapes.end(), shapes.begin(), shapes.end());
        }

        // add light
        void add_light(const std::shared_ptr<Light> &light)
        {
            m_lights.emplace_back(light);
        }

        // add lights
        void add_lights(const std::vector<std::shared_ptr<Light>> &lights)
        {
            m_lights.insert(m_lights.end(), lights.begin(), lights.end());
        }

        // get shapes
        [[nodiscard]] std::vector<std::shared_ptr<Shape>> &get_shapes()
        {
            return m_shapes;
        }

        // get lights
        [[nodiscard]] std::vector<std::shared_ptr<Light>> &get_lights()
        {
            return m_lights;
        }

        void remove_light(const std::shared_ptr<Light> &light)
        {
            auto it = std::find(m_lights.begin(), m_lights.end(), light);
            if (it != m_lights.end())
                m_lights.erase(it);
        }

        void remove_light(const int index)
        {
            if (index < m_lights.size())
                m_lights.erase(m_lights.begin() + index);
        }

        void remove_shape(const std::shared_ptr<Shape> &shape)
        {
            auto it = std::find(m_shapes.begin(), m_shapes.end(), shape);
            if (it != m_shapes.end())
                m_shapes.erase(it);
        }

        void remove_shape(const int index)
        {
            if (index < m_shapes.size())
                m_shapes.erase(m_shapes.begin() + index);
        }

        // get shapes
        [[nodiscard]] const std::vector<std::shared_ptr<Shape>> &get_shapes() const
        {
            return m_shapes;
        }

        // get lights
        [[nodiscard]] const std::vector<std::shared_ptr<Light>> &get_lights() const
        {
            return m_lights;
        }

        // get Max Depth
        int get_max_recurtion_level() const
        {
            return max_recurtion_level;
        }

        // set Max Depth
        void set_max_recurtion_level(const int max_depth)
        {
            max_recurtion_level = max_depth;
        }

        // get Samples Per Pixel
        int get_antialiasing_samples() const
        {
            return antialiasing_samples;
        }

        // set Samples Per Pixel
        void set_antialiasing_samples(const int new_samples_per_pixel)
        {
            antialiasing_samples = new_samples_per_pixel;
        }

        // serialize all data to a nlohmann json string object
        [[nodiscard]] std::string to_json() const noexcept
        {
            nlohmann::json json;

            json["max_recurtion_level"] = max_recurtion_level;

            json["antialiasing_samples"] = antialiasing_samples;

            nlohmann::json lights_json;
            for (const auto &light : m_lights)
                lights_json.emplace_back(nlohmann::json::parse(light->to_json()));
            json["lights"] = lights_json;

            nlohmann::json shapes_json;
            for (const auto &shape : m_shapes)
                shapes_json.emplace_back(nlohmann::json::parse(shape->to_json()));
            json["shapes"] = shapes_json;

            return json.dump();
        }

        // deserialize all data from a json string object
        void from_json(const std::string &json_string)
        {

            m_shapes.clear();
            m_lights.clear();

            nlohmann::json json = nlohmann::json::parse(json_string);

            max_recurtion_level = json["max_recurtion_level"];

            antialiasing_samples = json["antialiasing_samples"];

            for (const auto &light_json : json["lights"])
            {
                if (light_json["type"] == "PointLight")
                {
                    m_lights.emplace_back(PointLight::from_json(light_json.dump()));
                }
            }

            for (const auto &shape_json : json["shapes"])
            {
                if (shape_json["type"] == "Sphere")
                    m_shapes.emplace_back(Sphere::from_json(shape_json.dump()));
                else if (shape_json["type"] == "XZPlane")
                    m_shapes.emplace_back(XZPlane::from_json(shape_json.dump()));
                else if (shape_json["type"] == "YZPlane")
                    m_shapes.emplace_back(YZPlane::from_json(shape_json.dump()));
                else if (shape_json["type"] == "XYPlane")
                    m_shapes.emplace_back(XYPlane::from_json(shape_json.dump()));
                if (shape_json["type"] == "Cube")
                    m_shapes.emplace_back(Cube::from_json(shape_json.dump()));
            }
        }

    private:
        std::vector<std::shared_ptr<Shape>> m_shapes;
        std::vector<std::shared_ptr<Light>> m_lights;

        int max_recurtion_level = 7;
        int antialiasing_samples = 1;
    };

} // namespace Karbon
