#pragma once

#include "Constants.hpp"
#include "Matrix.hpp"
#include "Tuples/Color.hpp"
#include "Tuples/Point.hpp"
#include "Tuples/Vector.hpp"
#include "World.hpp"

namespace Karbon
{
    struct Camera
    {

        [[nodiscard]] Camera(int width, int height, float fov, Matrix4 transform = Karbon::IDENTITY) : m_width(width), m_height(height), m_field_of_view(fov), m_transform(transform)
        {
            set_pixel_size();
            m_transform = m_transform.inverse();
        }

        [[nodiscard]] void set_pixel_size()
        {
            PROFILE_FUNCTION();

            float half_view = std::tan(m_field_of_view / 2.0f);
            float aspect = m_width / (m_height + 0.0f);

            if (aspect >= 1.0)
            {
                m_half_width = half_view;
                m_half_height = half_view / aspect;
            }
            else
            {
                m_half_width = half_view / aspect;
                m_half_height = half_view;
            }

            m_pixel_size = (m_half_width * 2) / m_width;
        }

        [[nodiscard]] void transform(const Point &from, const Point &to, const Vector &up)
        {
            Vector forword = (to - from).normalize();

            Vector left = forword.cross(up).normalize();

            Vector new_up = left.cross(forword);

            Matrix4 orientation(left.x, left.y, left.z, 0,
                                new_up.x, new_up.y, new_up.z, 0,
                                -forword.x, -forword.y, -forword.z, 0,
                                0, 0, 0, 1);

            m_transform = orientation.translate(-from.x, -from.y, -from.z);

            m_inverse_transform = m_transform.inverse();
        }

        [[nodiscard]] Ray ray_for_pixel(float x, float y) const
        {
            PROFILE_FUNCTION();

            float xOffset = (x + 0.5f) * m_pixel_size;
            float yOffset = (y + 0.5f) * m_pixel_size;

            float world_x = m_half_width - xOffset;
            float world_y = m_half_height - yOffset;

            Point pixel = m_inverse_transform * Point(world_x, world_y, -1);
            Point origin = m_inverse_transform * Point(0, 0, 0);
            Vector direction = (pixel - origin).normalize();

            return Ray(origin, direction);
        }

        template <size_t width, size_t hight>
        [[nodiscard]] void render(World &w, Color (&canvas)[width][hight]) const
        {
            PROFILE_FUNCTION();

            for (int y = 0; y < m_height; y++)
            {
                std::cout << y << std::endl;

                for (int x = 0; x < m_width; x++)
                {
                    Color c;

                    if (w.get_antialiasing_samples() == 1)
                    {
                        Ray r = ray_for_pixel((float)x, (float)y);

                        c += w.color_at(r);
                    }
                    else
                    {
                        for (int i = 0; i < w.get_antialiasing_samples(); i++)
                        {
                            float u = (x + random<float>(-1, 1));
                            float v = (y + random<float>(-1, 1));

                            Ray r = ray_for_pixel(u, v);

                            c += w.color_at(r);
                        }

                        c *= (1.0f / w.get_antialiasing_samples());

                        c = Color::gamma_correct(c);
                    }

                    canvas[y][x] = c;
                }
            }
        }

        [[nodiscard]] std::shared_ptr<Color[]> render(const World &w)
        {
            PROFILE_FUNCTION();

            m_is_finished = false;

            debug_print("[RENDERER]: ", "Started Single-Threaded Rendering");

            Timer timer;

            std::shared_ptr<Color[]> image(new Color[m_width * m_height]);

            for (int y = 0; y < m_height; y++)
            {
                debug_print("[RENDERER]: ", "Thread {" + std::to_string(y + 1) + "}: Calculating Row: [" + std::to_string(y + 1) + '/' + std::to_string(m_height) + "]");

                for (int x = 0; x < m_width; x++)
                {
                    Color c;

                    if (w.get_antialiasing_samples() == 1)
                    {
                        Ray r = ray_for_pixel((float)x, (float)y);

                        c += w.color_at(r);
                    }
                    else
                    {
                        for (int i = 0; i < w.get_antialiasing_samples(); i++)
                        {
                            float u = (x + random<float>(-1, 1));
                            float v = (y + random<float>(-1, 1));

                            Ray r = ray_for_pixel(u, v);

                            c += w.color_at(r);
                        }

                        c *= (1.0f / w.get_antialiasing_samples());

                        c = Color::gamma_correct(c);
                    }

                    image.get()[y * m_width + x] = c;
                }
            }

            std::cout << "Finished Rendering in " << timer.elapsed() << " seconds" << std::endl;

            m_is_finished = true;

            debug_print("[RENDERER]: ", "Single-Threaded Rendering done in: " + std::to_string(timer.elapsed_millis()) + " ms");

            return image;
        }

        [[nodiscard]] std::shared_ptr<Color[]> render_multi_threaded(const World &w, const int thread_count = kCORE_COUNT)
        {
            PROFILE_FUNCTION();

            m_is_finished = false;

            debug_print("[RENDERER]: ", "Started Multi-Threaded Rendering");

            Timer timer;

            std::shared_ptr<Color[]> image(new Color[m_width * m_height]);

            std::vector<std::thread> threads;

            for (int i = 0; i < thread_count; i++)
            {
                const int index = i;

                threads.push_back(std::thread([&, index]()
                                              {
                    for (int y = index; y < m_height; y += thread_count)
                    {
                        debug_print("[RENDERER]: ","Thread {" + std::to_string(index + 1) + "}: Calculating Row: [" + std::to_string(y + 1) + '/' + std::to_string(m_height) + "]");

                        for (int x = 0; x < m_width; x++)
                        {
                            Color c;

                            if (w.get_antialiasing_samples() == 1)
                            {
                                Ray r = ray_for_pixel((float)x, (float)y);

                                c += w.color_at(r);

                                c = Color::scale(c, &map_to_range, 0, 255, 0, 1);

                                c = Color::gamma_correct(c);

                                c = Color::scale(c, &map_to_range, 0, 1, 0, 255);
                            } 
                            else
                            {
                                for (int i = 0; i < w.get_antialiasing_samples(); i++)
                                {
                                    float u = (x + random<float>(-1,1));
                                    float v = (y + random<float>(-1,1));

                                    Ray r = ray_for_pixel(u, v);

                                    c += w.color_at(r);
                                }

                                c *= (1.0f / w.get_antialiasing_samples());

                                c = Color::scale(c, &map_to_range, 0, 255, 0, 1);

                                c = Color::gamma_correct(c);

                                c = Color::scale(c, &map_to_range, 0, 1, 0, 255);
                            }
                            
                            image.get() [y *m_width + x] = c;
                        }
                    } }));
            }

            for (auto &t : threads)
                t.join();

            m_is_finished = true;

            debug_print("[RENDERER]: ", "Multi-Threaded Rendering done in: " + std::to_string(timer.elapsed_millis()) + " ms");

            return image;
        }

        // generate getters
        [[nodiscard]] constexpr int is_finished() const
        {
            return m_is_finished;
        }

        [[nodiscard]] constexpr int get_width() const
        {
            return m_width;
        }

        [[nodiscard]] constexpr int get_height() const
        {
            return m_height;
        }

        [[nodiscard]] constexpr float get_field_of_view() const
        {
            return m_field_of_view;
        }

        [[nodiscard]] constexpr float get_half_height() const
        {
            return m_half_height;
        }

        [[nodiscard]] constexpr float get_half_width() const
        {
            return m_half_width;
        }

        [[nodiscard]] constexpr float get_pixel_size() const
        {
            return m_pixel_size;
        }

        [[nodiscard]] constexpr const Vector &get_translation() const
        {
            return m_translation;
        }

        [[nodiscard]] constexpr const Matrix4 &get_transform() const
        {
            return m_transform;
        }

        [[nodiscard]] constexpr const Matrix4 &get_inverse_transform() const
        {
            return m_inverse_transform;
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

        // generate setters
        void set_width(int width)
        {
            m_width = width;
            set_pixel_size();
        }

        void set_height(int height)
        {
            m_height = height;
            set_pixel_size();
        }

        void set_field_of_view(float field_of_view)
        {
            m_field_of_view = field_of_view;
            set_pixel_size();
        }

        void constexpr set_half_height(float half_height)
        {
            m_half_height = half_height;
        }

        void constexpr set_half_width(float half_width)
        {
            m_half_width = half_width;
        }

        void constexpr set_pixel_size(float pixel_size)
        {
            m_pixel_size = pixel_size;
        }

        void constexpr set_transform(const Matrix4 &transform)
        {
            m_transform = transform;
        }

        void constexpr set_inverse_transform(const Matrix4 &inverse_transform)
        {
            m_inverse_transform = inverse_transform;
        }

        Camera &transform(const float (&translation)[3], const float (&rotation)[3])
        {
            m_translation = Vector((float)translation[0], (float)translation[1], (float)translation[2]);
            m_rotation_x = rotation[0];
            m_rotation_y = rotation[1];
            m_rotation_z = rotation[2];

            m_transform = Karbon::IDENTITY.translate(translation[0], translation[1], translation[2]).rotate(m_rotation_x, m_rotation_y, m_rotation_z);
            m_inverse_transform = m_transform.inverse();

            return *this;
        }

        Camera &transform_deg(const float (&translation)[3], const float (&rotation)[3])
        {
            m_translation = Vector((float)translation[0], (float)translation[1], (float)translation[2]);
            m_rotation_x = rotation[0] * (float)std::numbers::pi / 180.0f;
            m_rotation_y = rotation[1] * (float)std::numbers::pi / 180.0f;
            m_rotation_z = rotation[2] * (float)std::numbers::pi / 180.0f;

            m_transform = Karbon::IDENTITY.translate(translation[0], translation[1], translation[2]).rotate(m_rotation_x, m_rotation_y, m_rotation_z);
            m_inverse_transform = m_transform.inverse();

            return *this;
        }

        // serialize all data to a nlohmann json string object
        [[nodiscard]] std::string to_json() const noexcept
        {
            nlohmann::json j;

            j["width"] = m_width;
            j["height"] = m_height;
            j["field_of_view"] = m_field_of_view;
            j["transform"] = nlohmann::json::parse(m_transform.to_json());
            j["inverse_transform"] = nlohmann::json::parse(m_inverse_transform.to_json());

            return j.dump();
        }

        // deserialize all data from a json string object
        void from_json(const std::string &json)
        {
            nlohmann::json j = nlohmann::json::parse(json);

            m_width = j["width"];
            m_height = j["height"];
            m_field_of_view = j["field_of_view"];
            m_transform = Matrix4::from_json(j["transform"].dump());
            m_inverse_transform = Matrix4::from_json(j["inverse_transform"].dump());

            set_pixel_size();
        }

    private:
        bool m_is_finished = false;
        int m_width;
        int m_height;
        float m_field_of_view;
        float m_pixel_size;
        Vector m_translation = Vector(0, 0, 0);
        float m_rotation_x = 0;
        float m_rotation_y = 0;
        float m_rotation_z = 0;
        Matrix4 m_transform = Karbon::IDENTITY;
        Matrix4 m_inverse_transform = Karbon::IDENTITY;

        float m_half_width;
        float m_half_height;
    };
} // namespace Karbon
