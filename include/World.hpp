#pragma once

#include "Computation.hpp"
#include "Constants.hpp"
#include "Intersection.hpp"
#include "Lights/Light.hpp"
#include "Lights/PointLight.hpp"
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

            std::shared_ptr<Sphere> sphere = std::make_shared<Sphere>(Sphere());
            sphere->set_material(Material(Color(0.8f, 1, 0.6f), -1.0f, 0.7f, 0.2f, -1.0f, nullptr, 0.0f, -1.0f, -1.0f));
            m_shapes.emplace_back(sphere);

            std::shared_ptr<Sphere> sphere2 = std::make_shared<Sphere>(Sphere());
            sphere2->scale(0.5, 0.5, 0.5);
            m_shapes.emplace_back(sphere2);

            auto light = std::make_shared<PointLight>(PointLight());
            light->set_intensity(Color(1, 1, 1)).set_position(Point(-10, 10, -10));
            m_lights.emplace_back(light);
        }

        [[nodiscard]] std::vector<Intersection> intersects(const Ray &ray) const
        {
            PROFILE_FUNCTION();

            std::vector<Intersection> res;

            for (const auto &shape : m_shapes)
            {
                Intersection shape_xs = shape->intersects(ray);
                if (shape_xs.m_t > 0)
                    res.emplace_back(shape_xs);
            }

            // lamda std::vector sort
            std::sort(res.begin(), res.end(), [](const Intersection &a, const Intersection &b)
                      { return a.m_t < b.m_t; });

            return res;
        }

        [[nodiscard]] bool is_shadowed(const Point &point, const Light &light) const
        {
            PROFILE_FUNCTION();

            Vector v = light.m_position - point;
            float distance = v.magnitude();
            Vector direction = v.normalize();

            Ray ray(point, direction);
            auto xs = intersects(ray);

            Intersection hit = Intersection::hit(xs);

            if (hit.m_t >= 0 && hit.m_t < distance)
                return true;

            return false;
        }

        [[nodiscard]] Color color_at(const Ray &ray, const int light_samples = 0) const
        {
            auto xs = intersects(ray);
            Intersection hit = Intersection::hit(xs);

            // If we've exceeded the ray bounce limit, no more light is gathered.
            if (light_samples >= light_samples_per_pixel)
                return Karbon::BLACK;

            if (hit.m_t > 0)
            {
                const Shape *object = hit.m_object;
                Point p = ray.position(hit.m_t);
                Vector normalv = object->normal_at(p);

                Point over_point = p + normalv * kEpsilon;

                Ray rand_ray = Ray::random_in_unit_sphere(over_point);

                return 0.5 * color_at(rand_ray, light_samples + 1);
            }

            const float t = (float)(0.5 * (float)(ray.m_direction.y + 1.0));
            return (1.0f - t) * Color(255.0, 255.0, 255.0) + t * Color(127.5, 178.5, 255);
        }

        // [[nodiscard]] Color color_at(const Ray &ray, const int recursion_level = 0) const
        // {
        //     PROFILE_FUNCTION();

        //     if (recursion_level >= MAX_DEPTH)
        //         return Karbon::BLACK;

        //     auto xs = intersects(ray);
        //     Intersection hit = Intersection::hit(xs);

        //     if (hit.m_t < 0)
        //     {
        //         auto t = 0.5 * (ray.m_direction.y + 1.0);
        //         return (1.0 - t) * Color(255.0, 255.0, 255.0) + t * Color(0.5 * 255, 0.7 * 255, 1.0 * 255);
        //     }

        //     Computation comps = hit.prepare_computation(ray, xs);

        //     Color r = 0.5 * shade_hit(comps, recursion_level);

        //     if (light_samples_per_pixel > 1)
        //     {

        //         for (int i = 0; i < light_samples_per_pixel; i++)
        //         {
        //             const Shape *object = hit.m_object;
        //             Point p = ray.position(hit.m_t);
        //             Vector normalv = object->normal_at(p);

        //             Point over_point = p + normalv * kEpsilon;

        //             Ray rand_ray = Ray::random_in_unit_sphere_with_direction(over_point, normalv);

        //             Computation comps = hit.prepare_computation(rand_ray, xs);

        //             r += 0.5 * shade_hit(comps, recursion_level);
        //         }
        //     }

        //     return r;
        // }

        // [[nodiscard]] Color shade_hit(const Computation &comp, const int depth = 0) const
        // {

        //     Color res;

        //     for (const auto &light : m_lights)
        //     {
        //         bool in_shadow = is_shadowed(comp.m_over_point, *light);

        //         res = res + comp.m_s->get_material().lighting(
        //                         *light, *comp.m_s, comp.m_over_point, comp.m_eye_vector, comp.m_normal_vector, in_shadow);

        //         Color reflection_map = reflected_color(comp, depth + 1);

        //         Color refraction_map = refraction_color(comp, depth + 1);

        //         Material mat = comp.m_s->get_material();

        //         if (mat.get_reflectiveness() > 0 && mat.get_transparency() > 0)
        //         {
        //             float reflectiveness = comp.schilck();

        //             res = res + reflection_map * (1 - reflectiveness) + refraction_map * (1 - reflectiveness);
        //         }
        //         else
        //             res = res + reflection_map + refraction_map;
        //     }
        //     return res;
        // }

        // [[nodiscard]] Color reflected_color(const Computation &comp, const int recursion_level = 0) const
        // {
        //     PROFILE_FUNCTION();

        //     if (comp.m_s->get_material().get_reflectiveness() > 0)
        //     {
        //         Ray reflected_ray = Ray(comp.m_over_point, comp.m_reflection_vector);
        //         Color reflected_color = color_at(reflected_ray, recursion_level + 1);
        //         return reflected_color * comp.m_s->get_material().get_reflectiveness();
        //     }

        //     return Color();
        // }

        // [[nodiscard]] Color refraction_color(const Computation &comp, const int recursion_level = 0) const
        // {
        //     PROFILE_FUNCTION();

        //     if (comp.m_s->get_material().get_refractive_index() > 0)
        //     {
        //         float n_ratio = comp.m_inside ? comp.m_n1 / comp.m_n2 : comp.m_n2 / comp.m_n1;

        //         float cos_i = comp.m_eye_vector.dot(comp.m_normal_vector);

        //         float sin2_t = (n_ratio * n_ratio * (1.0f - cos_i * cos_i));

        //         if (sin2_t > 1.0)
        //             return Color();

        //         float cos_t = (float)sqrt(1.0 - sin2_t);

        //         Vector direction = comp.m_normal_vector * (n_ratio * cos_i - cos_t) - comp.m_eye_vector * n_ratio;

        //         Ray refracted_ray = Ray(comp.m_under_point, direction);

        //         return color_at(refracted_ray, recursion_level + 1) * comp.m_s->get_material().get_transparency();
        //     }

        //     return Color();
        // }

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
        int get_max_depth() const
        {
            return MAX_DEPTH;
        }

        // set Max Depth
        void set_max_depth(const int max_depth)
        {
            MAX_DEPTH = max_depth;
        }

        // get Samples Per Pixel
        int get_samples_per_pixel() const
        {
            return samples_per_pixel;
        }

        // set Samples Per Pixel
        void set_samples_per_pixel(const int new_samples_per_pixel)
        {
            samples_per_pixel = new_samples_per_pixel;
        }

        // get Samples Per Light
        int get_light_samples_per_pixel() const
        {
            return light_samples_per_pixel;
        }

        // set Samples Per Light
        void set_light_samples_per_pixel(const int new_light_samples_per_pixel)
        {
            light_samples_per_pixel = new_light_samples_per_pixel;
        }

        // serialize all data to a nlohmann json string object
        [[nodiscard]] std::string to_json() const noexcept
        {
            nlohmann::json json;

            json["max_depth"] = MAX_DEPTH;

            json["samples_per_pixel"] = samples_per_pixel;

            json["light_samples_per_pixel"] = light_samples_per_pixel;

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

            MAX_DEPTH = json["max_depth"];

            samples_per_pixel = json["samples_per_pixel"];

            light_samples_per_pixel = json["light_samples_per_pixel"];

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

        int MAX_DEPTH = 7;
        int samples_per_pixel = 10;
        int light_samples_per_pixel = 10;
    };

} // namespace Karbon
