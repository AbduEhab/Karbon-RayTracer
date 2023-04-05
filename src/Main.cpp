#include <Karbon.hpp>

auto scene = Karbon::Scene(Karbon::Camera(800, 600, (float)std::numbers::pi / 3), Karbon::World(0));

std::shared_ptr<Karbon::Color[]> canvas;
std::thread render_thread;

#ifdef _WIN32

#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Image.h"
#include "Walnut/Random.h"

#include <L2DFileDialog.h>

#include "Materials/Metal.hpp"

class MainLayer : public Walnut::Layer
{
public:
    virtual void OnUIRender() override
    {

        // ImGui::ShowDemoWindow();

        ImGui::Begin("World Outline");
        {
            static char *file_dialog_buffer = new char[500];
            static char path[500] = "";

            ImGui::TextUnformatted("Path: ");
            ImGui::InputText("##path", path, sizeof(path));
            ImGui::SameLine();

            if (FileDialog::file_dialog_open)
            {
                FileDialog::ShowFileDialog_s(&FileDialog::file_dialog_open, file_dialog_buffer, FileDialog::file_dialog_open_type);
            }

            if (ImGui::Button("Choose file"))
            {
                file_dialog_buffer = path;
                FileDialog::file_dialog_open = true;
                FileDialog::file_dialog_open_type = FileDialog::FileDialogType::OpenFile;
            }

            ImGui::SameLine();

            if (ImGui::Button("Load Scene"))
            {
                if (strlen(path) > 0)
                    scene.load_scene(path);
            }

            { // save scene

                static char name[32] = "Default_Scene";
                ImGui::InputText("Scene Name", name, sizeof(name), ImGuiInputTextFlags_CharsNoBlank);

                ImGui::SameLine();

                if (ImGui::Button("Save Scene"))
                {
                    if (strlen(name) > 0)
                        scene.save_scene(std::string(name) + ".json");
                }
            }
        }

        static size_t selected = 0;

        auto shapes = scene.m_world.get_shapes();
        auto lights = scene.m_world.get_lights();

        ImGui::Separator();

        ImGui::SetNextItemOpen(true, ImGuiCond_Once);
        if (ImGui::TreeNode("Objects"))
        {
            if (ImGui::Button("Remove"))
            {
                if (selected > shapes.size() - 1)
                    scene.m_world.remove_light(selected - shapes.size());
                else
                    scene.m_world.remove_shape(shapes[selected]);

                selected = 0;
            }

            ImGui::SetNextItemOpen(true, ImGuiCond_Once);
            if (ImGui::TreeNode("Shapes"))
            {
                {
                    if (ImGui::Button("Add Sphere"))
                    {
                        auto sphere = std::make_shared<Karbon::Sphere>(Karbon::Sphere());
                        scene.m_world.add_shape(sphere);
                    }

                    ImGui::SameLine();

                    if (ImGui::Button("Add Cube"))
                    {
                        auto cube = std::make_shared<Karbon::Cube>(Karbon::Cube());
                        scene.m_world.add_shape(cube);
                    }

                    if (ImGui::Button("Add XZ-Plane"))
                    {
                        auto plane = std::make_shared<Karbon::XZPlane>(Karbon::XZPlane());
                        scene.m_world.add_shape(plane);
                    }

                    ImGui::SameLine();

                    if (ImGui::Button("Add YZ-Plane"))
                    {
                        auto plane = std::make_shared<Karbon::YZPlane>(Karbon::YZPlane());
                        scene.m_world.add_shape(plane);
                    }

                    ImGui::SameLine();

                    if (ImGui::Button("Add XY-Plane"))
                    {
                        auto plane = std::make_shared<Karbon::XYPlane>(Karbon::XYPlane());
                        scene.m_world.add_shape(plane);
                    }
                }

                ImGui::Separator();

                for (int i = 0; i < shapes.size(); i++)
                {
                    if (ImGui::Selectable((shapes[i]->get_name() + std::to_string(i)).c_str()))
                        selected = i;
                }

                ImGui::TreePop(); // Shapes
            }

            ImGui::SetNextItemOpen(true, ImGuiCond_Once);
            if (ImGui::TreeNode("Lights"))
            {
                {
                    if (ImGui::Button("Add Point-Light"))
                    {
                        auto light = std::make_shared<Karbon::PointLight>(Karbon::PointLight());
                        scene.m_world.add_light(light);
                    }
                }

                ImGui::Separator();

                for (size_t i = shapes.size(), j = 0; i < shapes.size() + lights.size(); i++)
                {
                    if (ImGui::Selectable((lights[j++]->get_name() + std::to_string(i)).c_str()))
                        selected = i;
                }
                ImGui::TreePop(); // Lights
            }

            ImGui::TreePop(); // Objects
        }

        ImGui::Separator();

        ImGui::SetNextItemOpen(true, ImGuiCond_Once);
        if (ImGui::TreeNode("Details"))
        {
            {
                bool no_shapes = false;
                bool no_lights = false;
                ImGui::BeginGroup();
                if (shapes.size() == 0)
                {
                    ImGui::TextUnformatted("No shapes in scene");
                    no_shapes = true;
                }
                if (lights.size() == 0)
                {
                    ImGui::TextUnformatted("No lights in scene");
                    no_lights = true;
                }

                // ImGui::Text("Selected object: %s %d", selected < shapes.size() ? shapes[selected]->get_name() : lights[selected % lights.size()]->get_name(), selected);

                if (no_shapes && no_lights)
                    ImGui::Text("The scene is empty");
                else if (no_shapes && !no_lights)
                    ImGui::Text("Selected object: %s %d", lights[selected]->get_name(), selected);
                else if (no_lights && !no_shapes)
                    ImGui::Text("Selected object: %s %d", shapes[selected]->get_name(), selected);
                else
                    ImGui::Text("Selected object: %s %d", selected < shapes.size() ? shapes[selected]->get_name() : lights[selected % lights.size()]->get_name(), selected);

                ImGui::Separator();
                if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None))
                {
                    if (ImGui::BeginTabItem("Transform"))
                    {
                        if (selected < shapes.size())
                        {
                            auto shape = shapes[selected];

                            auto translations = shape->get_translation();
                            auto rotations = shape->get_rotations();
                            auto scales = shape->get_scale();

                            float transformation[3] = {translations.x, translations.y, translations.z};
                            float rotation[3] = {rotations.x, rotations.y, rotations.z};
                            float scale[3] = {scales.x, scales.y, scales.z};

                            // imgui text output
                            ImGui::Text("Translation: (x, y, z):");
                            ImGui::SliderFloat3("##Translation", transformation, -50, 50);

                            ImGui::Spacing();

                            ImGui::Text("Rotation: (x, y, z):");
                            ImGui::SliderFloat3("##Rotation", rotation, -180, 180);

                            ImGui::Spacing();
                            ImGui::Text("Scale: (x, y, z):");
                            ImGui::SliderFloat3("##Scale", scale, 0.1f, 10);

                            shape->transform_deg(transformation, rotation, scale);
                        }
                        else
                        {
                            if (lights.size() != 0)
                            {
                                auto light = lights[selected % lights.size()];

                                auto position_vec = light->get_position();

                                float position[3] = {position_vec.x, position_vec.y, position_vec.z};

                                ImGui::InputFloat3("Position", (float *)&position);

                                light->set_position(position);
                            }
                        }

                        ImGui::EndTabItem();
                    }

                    if (ImGui::BeginTabItem("Material"))
                    {
                        if (selected < shapes.size())
                        {
                            auto shape = shapes[selected];

                            int item_current = 0;

                            auto mat_type = shape->get_material();

                            if (!strcmp(mat_type->get_name(), "Metal"))
                            {
                                item_current = 1;
                            }
                            else if (!strcmp(mat_type->get_name(), "Dielectric"))
                            {
                                item_current = 2;
                            }

                            const char *materials[] = {"Lambertian", "Metal", "Dielectric"};
                            ImGui::Combo("Material Selection:", &item_current, materials, IM_ARRAYSIZE(materials));

                            float roughness = 0;
                            float ref_idx = 1.05f;

                            switch (item_current)
                            {
                            case 0:
                                if (strcmp(mat_type->get_name(), "Dielectric")) [[likely]]
                                    shape->set_material(std::make_shared<Karbon::Lambertian>(Karbon::Lambertian(*(Karbon::Lambertian *)mat_type.get())));
                                else [[unlikely]]
                                    shape->set_material(std::make_shared<Karbon::Lambertian>(Karbon::Lambertian()));

                                break;
                            case 1:
                                if (strcmp(mat_type->get_name(), "Dielectric")) [[likely]]
                                    shape->set_material(std::make_shared<Karbon::Metal>(Karbon::Metal(*(Karbon::Metal *)mat_type.get())));
                                else [[unlikely]]
                                    shape->set_material(std::make_shared<Karbon::Metal>(Karbon::Metal()));

                                roughness = ((Karbon::Metal *)shape->get_material().get())->get_roughness();

                                ImGui::SliderFloat("Roughness", &roughness, 0.0f, 1.0f);

                                ((Karbon::Metal *)shape->get_material().get())->set_roughness(roughness);

                                break;
                            case 2:
                                if (strcmp(mat_type->get_name(), "Dielectric")) [[likely]]
                                    shape->set_material(std::make_shared<Karbon::Dielectric>(Karbon::Dielectric(*(Karbon::Dielectric *)mat_type.get())));
                                else [[unlikely]]
                                    shape->set_material(std::make_shared<Karbon::Dielectric>(Karbon::Dielectric()));

                                ref_idx = shape->get_material().get()->get_refractive_index();

                                ImGui::SliderFloat("Refractive Index", &ref_idx, 1.05f, 3.0f);

                                shape->get_material().get()->set_refractive_index(ref_idx);

                                break;
                            default:
                                debug_print("[UI]: ", "Invalid material selection");
                            }

                            auto color = shape->get_material()->get_color();

                            float color_vec[3] = {color.r, color.g, color.b};

                            ImGui::ColorEdit3("Material Color", (float *)&color_vec);

                            shape->get_material()->set_color(color_vec);

                            // ---------------------------------

                            // auto pattern_type = shape->get_pattern();

                            // auto pattern = 4;

                            // if (!strcmp(pattern_type->get_name(), "Checker"))
                            // {
                            //     pattern = 0;
                            // }
                            // else if (!strcmp(pattern_type->get_name(), "Gradient"))
                            // {
                            //     pattern = 1;
                            // }
                            // else if (!strcmp(pattern_type->get_name(), "Ring"))
                            // {
                            //     pattern = 2;
                            // }
                            // else if (!strcmp(pattern_type->get_name(), "Stripe"))
                            // {
                            //     pattern = 3;
                            // }
                            // else if (!strcmp(pattern_type->get_name(), "None"))
                            // {
                            //     pattern = 4;
                            // }

                            // auto p_color = shape->get_pattern()->m_second_color;

                            // float p_color_vec[3] = {color.r, color.g, color.b};

                            // ImGui::ColorEdit3("Material Color", (float *)&p_color_vec);

                            // shape->get_pattern()->m_first_color = color_vec;

                            // shape->get_pattern()->m_second_color = p_color_vec;

                            // const char *patterns[] = {"Checker", "Gradient", "Ring", "Stripe", "None"};
                            // ImGui::Combo("Pattern Selection:", &item_current, patterns, IM_ARRAYSIZE(patterns));

                            // switch (pattern)
                            // {
                            // case 0:
                            //     if (strcmp(mat_type->get_name(), "Checker")) [[likely]]
                            //         shape->set_pattern(std::make_shared<Karbon::Checker>(Karbon::Checker(*(Karbon::Checker *)pattern_type.get())));
                            //     else [[unlikely]]
                            //         shape->set_pattern(std::make_shared<Karbon::Checker>(Karbon::Checker()));

                            //     break;
                            // case 1:
                            //     if (strcmp(mat_type->get_name(), "Gradient")) [[likely]]
                            //         shape->set_pattern(std::make_shared<Karbon::Gradient>(Karbon::Gradient(*(Karbon::Gradient *)pattern_type.get())));
                            //     else [[unlikely]]
                            //         shape->set_pattern(std::make_shared<Karbon::Gradient>(Karbon::Gradient()));

                            //     break;
                            // case 2:
                            //     if (strcmp(mat_type->get_name(), "Ring")) [[likely]]
                            //         shape->set_pattern(std::make_shared<Karbon::Ring>(Karbon::Ring(*(Karbon::Ring *)pattern_type.get())));
                            //     else [[unlikely]]
                            //         shape->set_pattern(std::make_shared<Karbon::Ring>(Karbon::Ring()));

                            //     break;
                            // case 3:
                            //     if (strcmp(mat_type->get_name(), "Stripe")) [[likely]]
                            //         shape->set_pattern(std::make_shared<Karbon::Stripe>(Karbon::Stripe(*(Karbon::Stripe *)pattern_type.get())));
                            //     else [[unlikely]]
                            //         shape->set_pattern(std::make_shared<Karbon::Stripe>(Karbon::Stripe()));

                            //     break;
                            // case 4:
                            //     if (strcmp(mat_type->get_name(), "None")) [[likely]]
                            //         shape->set_pattern(nullptr);
                            //     else [[unlikely]]
                            //         shape->set_pattern(nullptr);

                            //     break;
                            // default:
                            //     debug_print("[UI]: ", "Invalid pattern selection");
                            // }
                        }
                        else
                        {
                            auto light = lights[selected % lights.size()];

                            auto color = light->get_intensity() / 255;

                            float color_vec[3] = {color.r, color.g, color.b};

                            ImGui::ColorEdit3("Light Color", (float *)&color_vec);

                            light->set_SDR_intensity(color_vec);
                        }

                        ImGui::EndTabItem();
                    }

                    ImGui::EndTabBar();

                } // End of Detail view
            }

            ImGui::EndGroup();

            ImGui::TreePop(); // Details
        }

        ImGui::Separator();

        {
            ImGui::SetNextItemOpen(true, ImGuiCond_Once);
            if (ImGui::TreeNode("Render Settings"))
            {
                {
                    auto v_size = scene.m_camera.get_height();
                    auto h_size = scene.m_camera.get_width();

                    float size[2] = {(float)h_size, (float)v_size};

                    ImGui::Text("Resolution: (w, h):");

                    ImGui::InputFloat2("##Render Resolution", (float *)&size);

                    scene.m_camera.set_width((int)size[0]);
                    scene.m_camera.set_height((int)size[1]);
                }

                ImGui::Text("Sampling Settings:");

                {
                    auto render_depth = scene.m_world.get_max_recurtion_level();

                    ImGui::SliderInt("##Render Depth", &render_depth, 1, 100, "Render Depth : %d", ImGuiSliderFlags_Logarithmic);

                    scene.m_world.set_max_recurtion_level(render_depth);
                }

                {
                    int antialiasing_samples = scene.m_world.get_antialiasing_samples();

                    ImGui::SliderInt("##AA samples", &antialiasing_samples, 1, 100, "AA Samples: %d", ImGuiSliderFlags_Logarithmic);

                    scene.m_world.set_antialiasing_samples(antialiasing_samples);
                }

                ImGui::TreePop(); // Render Settings
            }
        }

        ImGui::Separator();

        {
            if (ImGui::Button("Render"))
            {
                Render();
            }

            ImGui::Text("Last render: %.3fms", m_LastRenderTime);
        }

        if (!is_first_render)
        {
            if (ImGui::Button("Save Render"))
            {
                Karbon::Timer timer;

                Karbon::save_image(canvas, scene.m_camera.get_width(), scene.m_camera.get_height(), "render.jpg");

                is_file_saved = true;

                m_file_save_time = timer.elapsed_millis();
            }

            if (is_file_saved)
            {
                ImGui::Text("File saved in %.3fms", m_file_save_time);
            }
        }

        ImGui::End(); // World Outline

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("Editor");

        m_ViewportWidth = scene.m_camera.get_width();
        m_ViewportHeight = scene.m_camera.get_height();

        if (m_Image)
            ImGui::Image(m_Image->GetDescriptorSet(), {(float)m_Image->GetWidth(), (float)m_Image->GetHeight()});

        ImGui::End();
        ImGui::PopStyleVar();
    }

    void Render()
    {
        PROFILE_FUNCTION();

        Karbon::Timer timer;

        is_first_render = false;

        // auto a2 = std::async([&]()
        //                      { return scene.m_camera.render_multi_threaded(scene.m_world, 3); });

        // canvas = a2.get();

        canvas = scene.m_camera.render_multi_threaded(scene.m_world);

        if (!m_Image || m_ViewportWidth != m_Image->GetWidth() || m_ViewportHeight != m_Image->GetHeight())
        {
            m_Image = std::make_shared<Walnut::Image>(m_ViewportWidth, m_ViewportHeight, Walnut::ImageFormat::RGBA);
            delete[] m_ImageData;
            m_ImageData = new uint32_t[m_ViewportWidth * m_ViewportHeight];
        }

        if (scene.m_camera.is_finished())
        {
            for (uint32_t i = 0; i < m_ViewportWidth * m_ViewportHeight; i++)
            {
                m_ImageData[i] = canvas.get()[i].create_ABGR();
            }
        }
        else
            for (uint32_t i = 0; i < m_ViewportWidth * m_ViewportHeight; i++)
                m_ImageData[i] = 0;

        m_Image->SetData(m_ImageData);

        is_file_saved = false;

        m_LastRenderTime = timer.elapsed_millis();
    }

private:
    std::shared_ptr<Walnut::Image> m_Image;
    uint32_t *m_ImageData = nullptr;
    uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

    float m_LastRenderTime = 0.0f;

    bool is_first_render = true;
    float m_file_save_time = 0.0f;
    bool is_file_saved = false;
};

Walnut::Application *Walnut::CreateApplication([[maybe_unused]] int argc, [[maybe_unused]] char **argv)
{
    Instrumentor::Get().beginSession("Main");

    scene.m_camera.transform(Karbon::Point(0, 1.5, -5), Karbon::Point(0, 1, 0), Karbon::Vector(0, 1, 0));

    Walnut::ApplicationSpecification spec;
    spec.Name = "Karbon Raytracer";

    Walnut::Application *app = new Walnut::Application(spec);
    app->PushLayer<MainLayer>();
    app->SetMenubarCallback([app]()
                            {
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit"))
			{
				app->Close();
			}
			ImGui::EndMenu();
		} });

    return app;
}

#else

int main()
{
    // Instrumentor::Get().beginSession("main");

    scene.m_camera.transform(Karbon::Point(0, 1.5, -5), Karbon::Point(0, 1, 0), Karbon::Vector(0, 1, 0));

    canvas = scene.m_camera.classic_render_multi_threaded(world, 4);

    Instrumentor::Get().endSession();

    // Get a console handle
    HWND myconsole = GetConsoleWindow();
    // Get a handle to device context
    HDC mydc = GetDC(myconsole);

    // Draw pixels
    for (int16_t i = 0; i < scene.m_camera.get_width(); i++)
    {

        for (int16_t j = 0; j < scene.m_camera.get_height(); j++)
        {
            COLORREF COLOR = RGB(canvas.get()[j * scene.m_camera.get_width() + i].r, canvas.get()[j * scene.m_camera.get_width() + i].g, canvas.get()[j * scene.m_camera.get_width() + i].b);

            SetPixel(mydc, i, j, COLOR);
        }
    }

    ReleaseDC(myconsole, mydc);

    std::cin.ignore();
    return 0;
}

#endif