#pragma once

#include "Constants.hpp"
#include "json.hpp"

namespace Karbon
{
    struct Color
    {
        [[nodiscard]] constexpr Color() : r(0), g(0), b(0), a(0xff){};

        [[nodiscard]] constexpr Color(float red, float green, float blue, int alpha = 0xff)
            : r(red > 255 ? 255 : red), g(green > 255 ? 255 : green), b(blue > 255 ? 255 : blue), a(alpha > 255 ? 255 : alpha){};

        [[nodiscard]] constexpr Color(const float (&color_array)[3]) : r(color_array[0] > 255 ? 255 : color_array[0]), g(color_array[1] > 255 ? 255 : color_array[1]), b(color_array[2] > 255 ? 255 : color_array[2]), a(0xff){};

        [[nodiscard]] constexpr Color(const float (&color_array)[4]) : r(color_array[0] > 255 ? 255 : color_array[0]), g(color_array[1] > 255 ? 255 : color_array[1]), b(color_array[2] > 255 ? 255 : color_array[3]), a(color_array[3] > 255 ? 255 : (int)color_array[3]){};

        [[nodiscard]] static constexpr Color create_SDR(float r, float g, float b) noexcept
        {
            return Color(r * 255, g * 255, b * 255);
        }

        [[nodiscard]] static constexpr Color create_SDR(const float (&color_array)[3]) noexcept
        {
            return Color(color_array[0] * 255, color_array[1] * 255, color_array[2] * 255);
        }

        [[nodiscard]] static constexpr Color create_SDR(const Color &c) noexcept
        {
            return Color(c.r * 255, c.g * 255, c.b * 255);
        }

        [[nodiscard]] static constexpr uint32_t create_RGBA(uint32_t r, uint32_t g, uint32_t b, uint32_t a) noexcept
        {
            return ((r & 0xff) << 24) + ((g & 0xff) << 16) + ((b & 0xff) << 8) + (a & 0xff);
        }

        [[nodiscard]] constexpr uint32_t create_RGBA() const noexcept
        {
            return (((int)r & 0xff) << 24) + (((int)g & 0xff) << 16) + (((int)b & 0xff) << 8) + ((int)a & 0xff);
        }

        [[nodiscard]] static constexpr uint32_t create_ABGR(uint32_t r, uint32_t g, uint32_t b, uint32_t a) noexcept
        {
            return (((uint32_t)a & 0xff) << 24) + (((uint32_t)b & 0xff) << 16) + (((uint32_t)g & 0xff) << 8) + ((uint32_t)r & 0xff);
        }

        [[nodiscard]] constexpr uint32_t create_ABGR() const noexcept
        {
            return (((uint32_t)a & 0xff) << 24) + (((uint32_t)b & 0xff) << 16) + (((uint32_t)g & 0xff) << 8) + ((uint32_t)r & 0xff);
        }

        [[nodiscard]] static constexpr Color clamp(const Color &c) noexcept
        {
            return Color(255 * std::clamp(c.r, 0.0f, 1.0f), 255 * std::clamp(c.g, 0.0f, 1.0f), 255 * std::clamp(c.b, 0.0f, 1.0f));
        }

        // apply gamma correction
        [[nodiscard]] static constexpr Color gamma_correct(const Color &c) noexcept
        {
            return Color(std::pow(c.r, 1.0f / 2.2f), std::pow(c.g, 1.0f / 2.2f), std::pow(c.b, 1.0f / 2.2f));
        }
        [[nodiscard]] static constexpr Color gamma_correct(const float (&color_array)[3]) noexcept
        {
            return Color(std::pow(color_array[0], 1.0f / 2.2f), std::pow(color_array[1], 1.0f / 2.2f), std::pow(color_array[2], 1.0f / 2.2f));
        }
        [[nodiscard]] static constexpr Color gamma_correct(const float (&color_array)[4]) noexcept
        {
            return Color(std::pow(color_array[0], 1.0f / 2.2f), std::pow(color_array[1], 1.0f / 2.2f), std::pow(color_array[2], 1.0f / 2.2f), (int)color_array[3]);
        }
        [[nodiscard]] static constexpr Color gamma_correct(const Color &c, float gamma) noexcept
        {
            return Color(std::pow(c.r, 1.0f / gamma), std::pow(c.g, 1.0f / gamma), std::pow(c.b, 1.0f / gamma));
        }
        [[nodiscard]] static constexpr Color gamma_correct(const float (&color_array)[3], float gamma) noexcept
        {
            return Color(std::pow(color_array[0], 1.0f / gamma), std::pow(color_array[1], 1.0f / gamma), std::pow(color_array[2], 1.0f / gamma));
        }

        // scale color with provided function
        template <typename... TArgs>
        [[nodiscard]] static constexpr Color scale(const Color &c, float (*f)(float, float, float, float, float), TArgs &&...args) noexcept
        {
            return Color(f(c.r, (float)(std::forward<TArgs>(args))...), f(c.g, (float)(std::forward<TArgs>(args))...), f(c.b, (float)(std::forward<TArgs>(args))...));
        }

        [[nodiscard]] int operator==(const Color &rhs) const noexcept
        {
            return (std::abs(r - rhs.r) <= kEpsilon) && (std::abs(g - rhs.g) <= kEpsilon) && (std::abs(b - rhs.b) <= kEpsilon) && (rhs.a == 0);
        }

        // / operator
        [[nodiscard]] constexpr Color operator/(const float &rhs) const noexcept
        {
            return Color(r / rhs, g / rhs, b / rhs);
        }

        constexpr Color &operator+=(const float rhs) noexcept
        {
            r = r + rhs;
            g = g + rhs;
            b = b + rhs;
            return *this;
        }

        constexpr Color &operator+=(const Color &rhs) noexcept
        {
            r += rhs.r;
            g += rhs.g;
            b += rhs.b;
            return *this;
        }

        constexpr Color &operator-=(const float rhs) noexcept
        {
            r = r - rhs;
            g = g - rhs;
            b = b - rhs;
            return *this;
        }

        constexpr Color &operator-=(const Color &rhs) noexcept
        {
            r -= rhs.r;
            g -= rhs.g;
            b -= rhs.b;
            return *this;
        }

        constexpr Color &operator*=(float factor) noexcept
        {
            r *= factor;
            g *= factor;
            b *= factor;
            return *this;
        }

        constexpr Color &operator*=(const Color &rhs) noexcept
        {
            r *= rhs.r;
            g *= rhs.g;
            b *= rhs.b;
            return *this;
        }

        [[nodiscard]] constexpr Color operator+(const Color &rhs) const noexcept
        {
            return Color(r + rhs.r, g + rhs.g, b + rhs.b);
        }

        [[nodiscard]] constexpr Color operator+(const float rhs) const noexcept
        {
            return Color(r + rhs, g + rhs, b + rhs);
        }

        [[nodiscard]] constexpr Color operator-(const float rhs) const noexcept
        {
            return Color(r - rhs, g - rhs, b - rhs);
        }

        [[nodiscard]] constexpr Color operator-(const Color &rhs) const noexcept
        {
            return Color(r - rhs.r, g - rhs.g, b - rhs.b);
        }

        [[nodiscard]] constexpr Color operator*(const float factor) const noexcept
        {
            return Color(r * factor, g * factor, b * factor);
        }

        [[nodiscard]] constexpr Color operator*(const Color &rhs) const noexcept
        {
            return Color(r * rhs.r, g * rhs.g, b * rhs.b);
        }

        friend std::ostream &operator<<(std::ostream &os, const Color &dt)
        {
            os << "(" << dt.r << ", " << dt.g << ", " << dt.b << ")";
            return os;
        };

        // serialize all data to a nlohmann json string object
        [[nodiscard]] std::string to_json() const noexcept
        {
            nlohmann::json j;
            j["r"] = r;
            j["g"] = g;
            j["b"] = b;
            j["a"] = a;
            return j.dump();
        }

        // static deserialize all data from a nlohmann json string object
        static Color from_json(const std::string &json_string)
        {
            nlohmann::json j = nlohmann::json::parse(json_string);
            return Color(j["r"], j["g"], j["b"], j["a"]);
        }

        float r;
        float g;
        float b;
        int a;
    };

    [[nodiscard]] constexpr Color operator*(const float t, const Color &c)
    {
        return Color(t * c.r, t * c.g, t * c.b);
    }

    static constexpr const Color BLACK = Color(0, 0, 0);
    static constexpr const Color GREY = Color(0.2f, 0.2f, 0.2f);
    static constexpr const Color WHITE = Color(1, 1, 1);
    static constexpr const Color RED = Color(1, 0, 0);
    static constexpr const Color GREEN = Color(0, 1, 0);
    static constexpr const Color BLUE = Color(0, 0, 1);
    static constexpr const Color PURPLE = Color(1, 0, 1);
    static constexpr const Color YELLOW = Color(1, 1, 0);
}