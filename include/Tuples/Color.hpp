#pragma once

#include "Constants.hpp"
#include "json.hpp"

namespace COAL
{
    struct Color
    {
        [[nodiscard]] constexpr Color() : r(0), g(0), b(0), a(0xff){};

        [[nodiscard]] constexpr Color(float red, float green, float blue, int alpha = 0xff)
            : r(red > 255 ? 255 : red), g(green > 255 ? 255 : green), b(blue > 255 ? 255 : blue), a(alpha > 255 ? 255 : alpha){};

        [[nodiscard]] constexpr Color(const float (&color_array)[3]) : r(color_array[0] > 255 ? 255 : color_array[0]), g(color_array[1] > 255 ? 255 : color_array[1]), b(color_array[2] > 255 ? 255 : color_array[2]), a(0xff){};

        [[nodiscard]] constexpr Color(const float (&color_array)[4]) : r(color_array[0] > 255 ? 255 : color_array[0]), g(color_array[1] > 255 ? 255 : color_array[1]), b(color_array[2] > 255 ? 255 : color_array[3]), a(color_array[3] > 255 ? 255 : (int)color_array[3]){};

        static constexpr Color create_SDR(float r, float g, float b) noexcept
        {
            return Color(r * 255, g * 255, b * 255);
        }

        static constexpr Color create_SDR(const float (&color_array)[3]) noexcept
        {
            return Color(color_array[0] * 255, color_array[1] * 255, color_array[2] * 255);
        }

        [[nodiscard]] int operator==(const Color &rhs) const noexcept
        {
            return (std::abs(r - rhs.r) <= kEpsilon) && (std::abs(g - rhs.g) <= kEpsilon) && (std::abs(b - rhs.b) <= kEpsilon) && (rhs.a == 0);
        }

        static constexpr uint32_t create_RGBA(uint32_t r, uint32_t g, uint32_t b, uint32_t a) noexcept
        {
            return ((r & 0xff) << 24) + ((g & 0xff) << 16) + ((b & 0xff) << 8) + (a & 0xff);
        }

        constexpr uint32_t create_RGBA() const noexcept
        {
            return (((int)r & 0xff) << 24) + (((int)g & 0xff) << 16) + (((int)b & 0xff) << 8) + ((int)a & 0xff);
        }

        static constexpr uint32_t create_ABGR(uint32_t r, uint32_t g, uint32_t b, uint32_t a) noexcept
        {
            return (((uint32_t)a & 0xff) << 24) + (((uint32_t)b & 0xff) << 16) + (((uint32_t)g & 0xff) << 8) + ((uint32_t)r & 0xff);
        }

        constexpr uint32_t create_ABGR() const noexcept
        {
            return (((uint32_t)a & 0xff) << 24) + (((uint32_t)b & 0xff) << 16) + (((uint32_t)g & 0xff) << 8) + ((uint32_t)r & 0xff);
        }

        // / operator
        constexpr Color operator/(const float &rhs) const noexcept
        {
            return Color(r / rhs, g / rhs, b / rhs);
        }

        [[nodiscard]] constexpr Color &operator+=(const float rhs) noexcept
        {
            r = r + rhs;
            g = g + rhs;
            b = b + rhs;
            return *this;
        }

        [[nodiscard]] constexpr Color &operator+=(const Color &rhs) noexcept
        {
            r += rhs.r;
            g += rhs.g;
            b += rhs.b;
            return *this;
        }

        [[nodiscard]] constexpr Color &operator-=(const float rhs) noexcept
        {
            r = r - rhs;
            g = g - rhs;
            b = b - rhs;
            return *this;
        }

        [[nodiscard]] constexpr Color &operator-=(const Color &rhs) noexcept
        {
            r -= rhs.r;
            g -= rhs.g;
            b -= rhs.b;
            return *this;
        }

        [[nodiscard]] constexpr Color &operator*=(float factor) noexcept
        {
            r *= factor;
            g *= factor;
            b *= factor;
            return *this;
        }

        [[nodiscard]] constexpr Color &operator*=(const Color &rhs) noexcept
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

    static constexpr const Color BLACK = Color(0, 0, 0);
    static constexpr const Color GREY = Color(0.2f, 0.2f, 0.2f);
    static constexpr const Color WHITE = Color(1, 1, 1);
    static constexpr const Color RED = Color(1, 0, 0);
    static constexpr const Color GREEN = Color(0, 1, 0);
    static constexpr const Color BLUE = Color(0, 0, 1);
    static constexpr const Color PURPLE = Color(1, 0, 1);
    static constexpr const Color YELLOW = Color(1, 1, 0);
}