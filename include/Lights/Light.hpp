#pragma once

#include "../Constants.hpp"
#include "./Tuples/Color.hpp"
#include "./Tuples/Vector.hpp"

namespace COAL
{
    struct Light
    {
        [[nodiscard]] constexpr Light() : m_intensity(COAL::Color()), m_position(COAL::Point()) {}

        [[nodiscard]] constexpr Light(COAL::Point &position, COAL::Color &intensity) : m_intensity(intensity), m_position(position) {}

        // abstract equality operator
        [[nodiscard]] virtual bool operator==(const Light &rhs) const noexcept = 0;

        // getters
        [[nodiscard]] constexpr const COAL::Color &get_intensity() const noexcept { return m_intensity; }
        [[nodiscard]] constexpr const COAL::Point &get_position() const noexcept { return m_position; }

        // setters
        constexpr Light &set_intensity(const COAL::Color &intensity) noexcept
        {
            this->m_intensity = intensity;
            return *this;
        }
        constexpr Light &set_position(const COAL::Point &position) noexcept
        {
            this->m_position = position;
            return *this;
        }

        // set m_intensity with a float[3]
        constexpr Light &set_intensity(const float (&intensity)[3]) noexcept
        {
            this->m_intensity = COAL::Color(intensity);
            return *this;
        }

        // set m_intensity with a float[3]
        constexpr Light &set_SDR_intensity(const float (&intensity)[3]) noexcept
        {
            this->m_intensity = COAL::Color::create_SDR(intensity);
            return *this;
        }

        // set m_position with a float[3]
        constexpr Light &set_position(const float (&position)[3]) noexcept
        {
            this->m_position = COAL::Point(position);
            return *this;
        }

        [[nodiscard]] virtual const char *get_name() const = 0;

        // serialize all data to a nlohmann json string object
        [[nodiscard]] virtual std::string to_json() const noexcept = 0;

        COAL::Color m_intensity;
        COAL::Point m_position;
    };
} // namespace COAL
