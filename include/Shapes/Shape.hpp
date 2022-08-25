#pragma once

#include "Constants.hpp"
#include "Intersection.hpp"
#include "Material.hpp"
#include "Matrix.hpp"
#include "Ray.hpp"
#include "Tuples/Point.hpp"
#include "Tuples/Vector.hpp"

namespace COAL
{

    struct Intersection;

    struct Shape
    {

        [[nodiscard]] virtual Intersection intersects(const Ray &ray) const = 0;

        [[nodiscard]] virtual Vector normal_at(const Point &p) const = 0;

        // setters and getters
        [[nodiscard]] const Material &get_material() const
        {
            return m_material;
        }

        [[nodiscard]] Material &get_material()
        {
            return m_material;
        }

        [[nodiscard]] constexpr const Matrix4 &get_transform() const
        {
            return m_transform;
        }

        [[nodiscard]] constexpr const Matrix4 &get_inverse_transform() const
        {
            return m_inverse_transform;
        }

        [[nodiscard]] constexpr const Matrix4 &get_normal_transform() const
        {
            return m_normal_transform;
        }

        [[nodiscard]] constexpr const Matrix4 &get_inverse_normal_transform() const
        {
            return m_inverse_normal_transform;
        }

        // abstract equality
        [[nodiscard]] virtual bool operator==(const Shape &other) const = 0;

        Shape &set_material(const Material &material)
        {
            m_material = material;

            return *this;
        }

        Shape &transform_no_rotation(const float (&translation)[3], const float (&rotation)[3], const float (&scale)[3])
        {
            m_translation = Vector((float)translation[0], (float)translation[1], (float)translation[2]);
            m_rotation_x = rotation[0];
            m_rotation_y = rotation[1];
            m_rotation_z = rotation[2];
            m_scale = Vector(scale[0], scale[1], scale[2]);

            m_transform = COAL::IDENTITY.translate(translation[0], translation[1], translation[2]).scale(scale[0], scale[1], scale[2]);
            m_inverse_transform = m_transform.inverse();
            m_normal_transform = m_inverse_transform.transpose();
            m_inverse_normal_transform = m_normal_transform.inverse();

            return *this;
        }

        Shape &transform(const float (&translation)[3], const float (&rotation)[3], const float (&scale)[3])
        {
            m_translation = Vector((float)translation[0], (float)translation[1], (float)translation[2]);
            m_rotation_x = rotation[0];
            m_rotation_y = rotation[1];
            m_rotation_z = rotation[2];
            m_scale = Vector(scale[0], scale[1], scale[2]);

            m_transform = COAL::IDENTITY.translate(translation[0], translation[1], translation[2]).scale(scale[0], scale[1], scale[2]).rotate(m_rotation_x, m_rotation_y, m_rotation_z);
            m_inverse_transform = m_transform.inverse();
            m_normal_transform = m_inverse_transform.transpose();
            m_inverse_normal_transform = m_normal_transform.inverse();

            return *this;
        }

        Shape &transform_deg(const float (&translation)[3], const float (&rotation)[3], const float (&scale)[3])
        {
            m_translation = Vector((float)translation[0], (float)translation[1], (float)translation[2]);
            m_rotation_x = rotation[0] * (float)std::numbers::pi / 180.0f;
            m_rotation_y = rotation[1] * (float)std::numbers::pi / 180.0f;
            m_rotation_z = rotation[2] * (float)std::numbers::pi / 180.0f;
            m_scale = Vector(scale[0], scale[1], scale[2]);

            m_transform = COAL::IDENTITY.translate(translation[0], translation[1], translation[2]).scale(scale[0], scale[1], scale[2]).rotate(m_rotation_x, m_rotation_y, m_rotation_z);
            m_inverse_transform = m_transform.inverse();
            m_normal_transform = m_inverse_transform.transpose();
            m_inverse_normal_transform = m_normal_transform.inverse();

            return *this;
        }

        // translate the shape
        Shape &translate(const Vector &t)
        {
            m_translation = t;
            m_transform = COAL::IDENTITY.translate(t.x, t.y, t.z);
            m_inverse_transform = m_transform.inverse();
            m_normal_transform = m_inverse_transform.transpose();
            m_inverse_normal_transform = m_normal_transform.inverse();

            return *this;
        }

        // translate the shape
        Shape &translate(const float x, const float y, const float z)
        {
            m_translation = Vector(x, y, z);
            m_transform = COAL::IDENTITY.translate(x, y, z);
            m_inverse_transform = m_transform.inverse();
            m_normal_transform = m_inverse_transform.transpose();
            m_inverse_normal_transform = m_normal_transform.inverse();

            return *this;
        }

        // scale the shape
        Shape &scale(const Vector &s)
        {
            m_scale = s;
            m_transform = m_transform.scale(s.x, s.y, s.z);
            m_inverse_transform = m_transform.inverse();
            m_normal_transform = m_inverse_transform.transpose();
            m_inverse_normal_transform = m_normal_transform.inverse();

            return *this;
        }

        // scale the shape
        Shape &scale(const float x, const float y, const float z)
        {
            m_scale = Vector(x, y, z);
            m_transform = m_transform.scale(x, y, z);
            m_inverse_transform = m_transform.inverse();
            m_normal_transform = m_inverse_transform.transpose();
            m_inverse_normal_transform = m_normal_transform.inverse();

            return *this;
        }

        // rotate the shape
        Shape &rotate_x(const float radians)
        {
            m_rotation_x = radians;
            m_transform = m_transform.rotate_x(radians);
            m_inverse_transform = m_transform.inverse();
            m_normal_transform = m_inverse_transform.transpose();
            m_inverse_normal_transform = m_normal_transform.inverse();

            return *this;
        }

        Shape &rotate_y(const float radians)
        {
            m_rotation_y = radians;
            m_transform = m_transform.rotate_y(radians);
            m_inverse_transform = m_transform.inverse();
            m_normal_transform = m_inverse_transform.transpose();
            m_inverse_normal_transform = m_normal_transform.inverse();

            return *this;
        }

        Shape &rotate_z(const float radians)
        {
            m_rotation_z = radians;
            m_transform = m_transform.rotate_z(radians);
            m_inverse_transform = m_transform.inverse();
            m_normal_transform = m_inverse_transform.transpose();
            m_inverse_normal_transform = m_normal_transform.inverse();

            return *this;
        }

        // get name
        [[nodiscard]] virtual const char *get_name() const = 0;

        // getters
        [[nodiscard]] constexpr const Vector &get_translation() const
        {
            return m_translation;
        }

        [[nodiscard]] constexpr const Vector &get_scale() const
        {
            return m_scale;
        }

        // get rotations
        [[nodiscard]] constexpr Vector get_rotations() const
        {
            return {m_rotation_x, m_rotation_y, m_rotation_z};
        }

        [[nodiscard]] constexpr float get_rotation_x() const
        {
            return m_rotation_x;
        }

        [[nodiscard]] constexpr float get_rotation_y() const
        {
            return m_rotation_y;
        }

        [[nodiscard]] constexpr float get_rotation_z() const
        {
            return m_rotation_z;
        }

        // serialize all data to a nlohmann json string object
        [[nodiscard]] virtual std::string to_json() const noexcept = 0;

    private:
        // private:
        COAL::Material m_material = COAL::Material();
        COAL::Matrix4 m_transform = COAL::IDENTITY;
        COAL::Matrix4 m_inverse_transform = COAL::IDENTITY;
        COAL::Matrix4 m_normal_transform = COAL::IDENTITY;
        COAL::Matrix4 m_inverse_normal_transform = COAL::IDENTITY;
        Vector m_translation = Vector(0, 0, 0);
        Vector m_scale = Vector(1, 1, 1);
        float m_rotation_x = 0;
        float m_rotation_y = 0;
        float m_rotation_z = 0;
    };

    [[nodiscard]] COAL::Color Pattern::colot_at(const Shape &s, const COAL::Point &p) const
    {
        PROFILE_FUNCTION();

        Point object_point = s.get_transform().inverse() * p;
        Point pattern_point = m_transform.inverse() * object_point;

        return color_at(pattern_point);
    }

} // namespace COAL
