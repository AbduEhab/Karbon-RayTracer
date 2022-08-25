#pragma once

#include "Vector.hpp"
#include "json.hpp"

namespace COAL
{
    struct Point
    {
        [[nodiscard]] constexpr Point() : x(0), y(0), z(0){};

        [[nodiscard]] constexpr Point(float x, float y, float z) : x(x), y(y), z(z){};

        [[nodiscard]] constexpr Point(const float (&vector_array)[3]) : x(vector_array[0]), y(vector_array[1]), z(vector_array[2]){};

        [[nodiscard]] bool operator==(const Point &rhs) const noexcept
        {
            return (std::abs(x - rhs.x) <= kEpsilon) && (std::abs(y - rhs.y) <= kEpsilon) && (std::abs(z - rhs.z) <= kEpsilon);
        }

        [[nodiscard]] constexpr Point &operator+=(const float rhs) noexcept
        {
            x = x + rhs;
            y = x + rhs;
            z = x + rhs;
            return *this;
        }

        [[nodiscard]] constexpr Point &operator+=(const Point &rhs) noexcept
        {
            x += rhs.x;
            y += rhs.y;
            z += rhs.z;
            return *this;
        }

        [[nodiscard]] constexpr Point &operator-=(const float rhs) noexcept
        {
            x = x - rhs;
            y = y - rhs;
            z = z - rhs;
            return *this;
        }

        // - point
        [[nodiscard]] constexpr Point operator-() const noexcept
        {
            return Point(-x, -y, -z);
        }

        // subtract a point from a point
        [[nodiscard]] constexpr Vector operator-(const Point &rhs) const noexcept
        {
            return Vector(x - rhs.x, y - rhs.y, z - rhs.z);
        }

        // subtract a vector from a point
        [[nodiscard]] constexpr Point operator-(const Vector &rhs) const noexcept
        {
            return Point(x - rhs.x, y - rhs.y, z - rhs.z);
        }

        // add a point to a Vector
        [[nodiscard]] constexpr Point operator+(const Vector &rhs) const noexcept
        {
            return Point(x + rhs.x, y + rhs.y, z + rhs.z);
        }

        // [] operator
        [[nodiscard]] constexpr float operator[](const char i) const noexcept
        {
            assert(i >= 0 && i < 3);

            switch (i)
            {
            case 0:
                return x;
            case 1:
                return y;
            case 2:
                return z;
            default:
                break;
            }

            assert(false);
            return -1;
        }

        [[nodiscard]] constexpr Point &operator*=(const float rhs) noexcept
        {
            x = x * rhs;
            y = y * rhs;
            z = z * rhs;
            return *this;
        }

        [[nodiscard]] constexpr Point &operator/=(const float rhs) noexcept
        {
            x = x / rhs;
            y = y / rhs;
            z = z / rhs;
            return *this;
        }

        [[nodiscard]] constexpr Point &operator*=(const Point &rhs) noexcept
        {
            x = x * rhs.x;
            y = y * rhs.y;
            z = z * rhs.z;
            return *this;
        }

        [[nodiscard]] constexpr Point &operator/=(const Point &rhs) noexcept
        {
            x = x / rhs.x;
            y = y / rhs.y;
            z = z / rhs.z;
            return *this;
        }

        [[nodiscard]] constexpr Point &operator*=(const Vector &rhs) noexcept
        {
            x = x * rhs.x;
            y = y * rhs.y;
            z = z * rhs.z;
            return *this;
        }

        [[nodiscard]] constexpr Point &operator/=(const Vector &rhs) noexcept
        {
            x = x / rhs.x;
            y = y / rhs.y;
            z = z / rhs.z;
            return *this;
        }

        friend std::ostream &operator<<(std::ostream &os, const Point &dt)
        {
            os << "(" << dt.x << ", " << dt.y << ", " << dt.z << ")";
            return os;
        };

        // serialize all data to a nlohmann json string object
        [[nodiscard]] std::string to_json() const noexcept
        {
            nlohmann::json j;
            j["x"] = x;
            j["y"] = y;
            j["z"] = z;
            return j.dump();
        }

        // static deserialize all data from a nlohmann json string object
        static Point from_json(const std::string &json_string)
        {
            nlohmann::json j = nlohmann::json::parse(json_string);
            return Point(j["x"], j["y"], j["z"]);
        }
        

        float x;
        float y;
        float z;
    };

}