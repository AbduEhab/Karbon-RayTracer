#pragma once

#include "../Constants.hpp"
#include "./Tuples/Color.hpp"
#include "./Tuples/Vector.hpp"

namespace Karbon
{
    struct Light
    {
        [[nodiscard]] constexpr Light() : m_intensity(Karbon::Color()), m_position(Karbon::Point()) {}

        [[nodiscard]] constexpr Light(Karbon::Point &position, Karbon::Color &intensity) : m_intensity(intensity), m_position(position) {}

        // abstract equality operator
        [[nodiscard]] virtual bool operator==(const Light &rhs) const noexcept = 0;

        // getters
        [[nodiscard]] constexpr const Karbon::Color &get_intensity() const noexcept { return m_intensity; }
        [[nodiscard]] constexpr const Karbon::Point &get_position() const noexcept { return m_position; }

        // setters
        constexpr Light &set_intensity(const Karbon::Color &intensity) noexcept
        {
            this->m_intensity = intensity;
            return *this;
        }
        constexpr Light &set_position(const Karbon::Point &position) noexcept
        {
            this->m_position = position;
            return *this;
        }

        // set m_intensity with a float[3]
        constexpr Light &set_intensity(const float (&intensity)[3]) noexcept
        {
            this->m_intensity = Karbon::Color(intensity);
            return *this;
        }

        // set m_intensity with a float[3]
        constexpr Light &set_SDR_intensity(const float (&intensity)[3]) noexcept
        {
            this->m_intensity = Karbon::Color::create_SDR(intensity);
            return *this;
        }

        // set m_position with a float[3]
        constexpr Light &set_position(const float (&position)[3]) noexcept
        {
            this->m_position = Karbon::Point(position);
            return *this;
        }

        [[nodiscard]] virtual const char *get_name() const = 0;

        // serialize all data to a nlohmann json string object
        [[nodiscard]] virtual std::string to_json() const noexcept = 0;

        Karbon::Color m_intensity;
        Karbon::Point m_position;
    };
} // namespace Karbon
