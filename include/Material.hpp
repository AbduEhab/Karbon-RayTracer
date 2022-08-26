#pragma once

#include "Patterns/Pattern.hpp"
#include "Shapes/Shape.hpp"
#include "Tuples/Color.hpp"
#include "json.hpp"

// #include "Patterns/Gradient.hpp"
// #include "Patterns/Checker.hpp"
// #include "Patterns/Ring.hpp"
// #include "Patterns/Stripe.hpp"

namespace Karbon
{

    struct Shape;

    struct Material
    {

        [[nodiscard]] constexpr Material() : m_pattern(nullptr){};

        [[nodiscard]] Material(const Color &color, const float ambient, const float diffuse, const float specular, const float shininess, const std::shared_ptr<Pattern> pattern, const float reflectiveness, const float transparency, const float refractiveIndex) : m_pattern(pattern)
        {
            if (color.r < 0 || color.r > 255 || color.g < 0 || color.g > 255 || color.b < 0 || color.b > 255)
            {
                std::cout << "invalid color recived " << __FUNCTION__ << std::endl;
            }
            else
            {
                m_color = color;
            }

            if (ambient >= 0 && ambient <= 1)
            {
                this->m_ambient = ambient;
            }
            else
            {
                std::cout << "invalid ambient recived " << __FUNCTION__ << std::endl;
            }

            if (diffuse >= 0 && diffuse <= 1)
            {
                this->m_diffuse = diffuse;
            }
            else
            {
                std::cout << "invalid diffuse recived " << __FUNCTION__ << std::endl;
            }

            if (specular >= 0 && specular <= 1)
            {
                this->m_specular = specular;
            }
            else
            {
                std::cout << "invalid specular recived " << __FUNCTION__ << std::endl;
            }

            if (shininess >= 0 && shininess <= 200)
            {
                this->m_shininess = shininess;
            }
            else
            {
                std::cout << "invalid shininess recived " << __FUNCTION__ << std::endl;
            }

            m_reflectiveness = reflectiveness > 1 ? 1 : reflectiveness < 0 ? 0
                                                                           : reflectiveness;

            m_transparency = transparency >= 0 ? transparency : 0;

            m_refractive_index = refractiveIndex >= 0 ? refractiveIndex : 0;
        }

        // lighting
        [[nodiscard]] Color lighting(const Light &light, const Shape &shape, const Point &point, const Vector &eyev, const Vector &normalv, const bool inShadow) const
        {
            PROFILE_FUNCTION();

            Color effColor;

            if (m_pattern)
            {
                effColor = m_pattern->colot_at(shape, point) * light.m_intensity;
            }
            else
            {
                effColor = m_color * light.m_intensity;
            }

            Vector lightv = (light.m_position - point).normalize();

            Color ambient = effColor * m_ambient;

            float lightDotNormal = lightv.dot(normalv);

            Color diffuse;
            Color specular;

            if (!inShadow && lightDotNormal >= 0)
            {
                diffuse = effColor * m_diffuse * lightDotNormal;

                Vector reflectv = -lightv.reflect(normalv);
                float reflectDotEye = reflectv.dot(eyev);

                if (reflectDotEye <= 0)
                {
                    specular = Color(0, 0, 0);
                }
                else
                {
                    float factor = std::pow(reflectDotEye, m_shininess);
                    specular = light.m_intensity * m_specular * factor;
                }
            }

            return ambient + diffuse + specular;
        }

        // == operator
        [[nodiscard]] bool operator==(const Material &rhs) const noexcept
        {
            return (m_color == rhs.m_color) && (m_ambient == rhs.m_ambient) && (m_diffuse == rhs.m_diffuse) && (m_specular == rhs.m_specular) && (m_shininess == rhs.m_shininess) && (m_reflectiveness == rhs.m_reflectiveness) && (m_transparency == rhs.m_transparency) && (m_refractive_index == rhs.m_refractive_index);
        }

        // << operator
        friend std::ostream &operator<<(std::ostream &os, const Material &m)
        {
            os << "material: " << std::endl;
            os << "ambient: " << m.m_ambient << std::endl;
            os << "diffuse: " << m.m_diffuse << std::endl;
            os << "specular: " << m.m_specular << std::endl;
            os << "shininess: " << m.m_shininess << std::endl;
            os << "reflectiveness: " << m.m_reflectiveness << std::endl;
            os << "transparency: " << m.m_transparency << std::endl;
            os << "refractive index: " << m.m_refractive_index << std::endl;
            os << "pattern: " << m.m_pattern << std::endl;
            return os;
        }

        // getters
        [[nodiscard]] constexpr float get_ambient() const noexcept { return m_ambient; }
        [[nodiscard]] constexpr float get_diffuse() const noexcept { return m_diffuse; }
        [[nodiscard]] constexpr float get_specular() const noexcept { return m_specular; }
        [[nodiscard]] constexpr float get_shininess() const noexcept { return m_shininess; }
        [[nodiscard]] constexpr float get_reflectiveness() const noexcept { return m_reflectiveness; }
        [[nodiscard]] constexpr float get_transparency() const noexcept { return m_transparency; }
        [[nodiscard]] constexpr float get_refractive_index() const noexcept { return m_refractive_index; }
        [[nodiscard]] constexpr Color get_color() const noexcept { return m_color; }
        [[nodiscard]] const std::shared_ptr<Pattern> get_pattern() const noexcept { return m_pattern; }

        // setters
        constexpr Material &set_ambient(const float ambient) noexcept
        {
            if (ambient >= 0 && ambient <= 1)
            {
                this->m_ambient = ambient;
            }
            else
            {
                std::cout << "invalid ambient recived " << __FUNCTION__ << std::endl;
            }

            return *this;
        }

        constexpr Material &set_diffuse(const float diffuse) noexcept
        {
            if (diffuse >= 0 && diffuse <= 1)
            {
                this->m_diffuse = diffuse;
            }
            else
            {
                std::cout << "invalid diffuse recived " << __FUNCTION__ << std::endl;
            }
            return *this;
        }

        constexpr Material &set_specular(const float specular) noexcept
        {
            if (specular >= 0 && specular <= 1)
            {
                this->m_specular = specular;
            }
            else
            {
                std::cout << "invalid specular recived " << __FUNCTION__ << std::endl;
            }

            return *this;
        }

        constexpr Material &set_shininess(const float shininess) noexcept
        {
            if (shininess >= 0 && shininess <= 200)
            {
                this->m_shininess = shininess;
            }
            else
            {
                std::cout << "invalid shininess recived " << __FUNCTION__ << std::endl;
            }

            return *this;
        }

        constexpr Material &set_reflectiveness(const float reflectiveness) noexcept
        {
            m_reflectiveness = reflectiveness > 1 ? 1 : reflectiveness < 0 ? 0
                                                                           : reflectiveness;

            return *this;
        }

        constexpr Material &set_transparency(const float transparency) noexcept
        {
            m_transparency = transparency >= 0 ? transparency : 0;

            return *this;
        }

        constexpr Material &set_refractive_index(const float refractiveIndex) noexcept
        {
            m_refractive_index = refractiveIndex >= 0 ? refractiveIndex : 0;

            return *this;
        }

        constexpr Material &set_color(const Color &color) noexcept
        {
            m_color = color;

            return *this;
        }

        constexpr Material &set_color(const float (&color_array)[3]) noexcept
        {
            m_color = Color(color_array[0], color_array[1], color_array[2]);

            return *this;
        }

        constexpr Material &set_color(const float r, const float g, const float b) noexcept
        {
            m_color = Color(r, g, b);

            return *this;
        }

        Material &set_pattern(std::shared_ptr<Pattern> pattern) noexcept
        {
            m_pattern = pattern;

            return *this;
        }

        // serialize all data to a nlohmann json string object
        [[nodiscard]] std::string to_json() const noexcept
        {
            nlohmann::json json_obj;
            json_obj["color"] = nlohmann::json::parse(m_color.to_json());
            json_obj["ambient"] = m_ambient;
            json_obj["diffuse"] = m_diffuse;
            json_obj["specular"] = m_specular;
            json_obj["shininess"] = m_shininess;
            json_obj["reflectiveness"] = m_reflectiveness;
            json_obj["transparency"] = m_transparency;
            json_obj["refractive index"] = m_refractive_index;

            if (m_pattern)
            {
                json_obj["pattern"] = nlohmann::json::parse(m_pattern->to_json());
            }
            else
            {
                json_obj["pattern"] = nullptr;
            }
            return json_obj.dump();
        }

        // static deserialize all data from a nlohmann json string object
        static Material from_json(const std::string &json_str)
        {
            nlohmann::json json_obj = nlohmann::json::parse(json_str);

            Color color = Color::from_json(json_obj["color"].dump());
            float ambient = json_obj["ambient"];
            float diffuse = json_obj["diffuse"];
            float specular = json_obj["specular"];
            float shininess = json_obj["shininess"];
            float reflectiveness = json_obj["reflectiveness"];
            float transparency = json_obj["transparency"];
            float refractive_index = json_obj["refractive index"];
            std::shared_ptr<Pattern> pattern;
            if (json_obj["pattern"] != nullptr)
            {
                // if (json_obj["pattern"] == "Checker")
                // {
                //     pattern = Checker::from_json(json_obj["pattern"].dump()));
                // }
                // else if (json_obj["pattern"] == "Stripe")
                // {
                //     pattern = Stripe::from_json(json_obj["pattern"].dump()));
                // }
                // else if (json_obj["pattern"] == "Ring")
                // {
                //     pattern = Ring::from_json(json_obj["pattern"].dump()));
                // }
                // else if (json_obj["pattern"] == "Gradient")
                // {
                //     pattern = Gradient::from_json(json_obj["pattern"].dump()));
                // }
            }
            else
            {
                pattern = nullptr;
            }
            return Material(color, ambient, diffuse, specular, shininess, pattern, reflectiveness, transparency,
                            refractive_index);
        }

    private:
        Color m_color = Karbon::WHITE;
        std::shared_ptr<Pattern> m_pattern;
        float m_ambient = 0.1f;
        float m_diffuse = 0.9f;
        float m_specular = 0.9f;
        float m_shininess = 200.0;
        float m_reflectiveness = 0.0;
        float m_transparency = 0.0;
        float m_refractive_index = 1.0;
    };
}; // namespace Karbon
