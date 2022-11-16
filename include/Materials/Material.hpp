#pragma once

#include "Ray.hpp"
#include "Tuples/Color.hpp"

#include "Computation.hpp"

namespace Karbon
{
    struct Computation;

    struct Material
    {

        virtual bool scatter(const Ray &r_in, const Computation &comp, Color &attenuation, Ray &scattered) const = 0;

        // get color
        [[nodiscard]] constexpr const Color &get_color() const noexcept
        {
            return m_color;
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

        [[nodiscard]] constexpr float get_refractive_index() const noexcept
        {
            return m_refractive_index;
        }

        constexpr Material &set_refractive_index(const float refractive_index) noexcept
        {
            m_refractive_index = refractive_index;

            return *this;
        }

        [[nodiscard]] virtual const char *get_name() const = 0;

        // serialize all data to a nlohmann json string object
        [[nodiscard]] virtual std::string to_json() const = 0;

    private:
        Color m_color = Karbon::WHITE;
        float m_refractive_index = 1;
    };
}; // namespace Karbon
