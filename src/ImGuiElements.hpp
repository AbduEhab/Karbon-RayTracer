#pragma once

#include "karbon.hpp"

#ifdef _WIN32

#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Image.h"
#include "Walnut/Random.h"

#include <L2DFileDialog.h>

void render_details_tree()
{
    ImGui::SetNextItemOpen(true, ImGuiCond_Once);
    if (ImGui::TreeNode("Details"))
    {

        ImGui::BeginGroup();
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
                    auto light = lights[selected % lights.size()];

                    auto position_vec = light->get_position();

                    float position[3] = {position_vec.x, position_vec.y, position_vec.z};

                    ImGui::InputFloat3("Position", (float *)&position);

                    light->set_position(position);
                }

                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Material"))
            {
                if (selected < shapes.size())
                {
                    auto shape = shapes[selected];

                    auto color = shape->get_material().get_color();
                    auto specular = shape->get_material().get_specular();
                    auto diffuse = shape->get_material().get_diffuse();
                    auto reflectiveness = shape->get_material().get_reflectiveness();
                    auto shininess = shape->get_material().get_shininess();
                    auto ambient = shape->get_material().get_ambient();
                    auto transparency = shape->get_material().get_transparency();
                    auto refractive_index = shape->get_material().get_refractive_index();
                    auto pattern = shape->get_material().get_pattern();

                    float color_vec[3] = {color.r, color.g, color.b};

                    ImGui::ColorEdit3("Material Color", (float *)&color_vec);

                    ImGui::SliderFloat("Specular", &specular, 0.0f, 1.0f);
                    ImGui::SliderFloat("Diffuse", &diffuse, 0.0f, 1.0f);
                    ImGui::SliderFloat("Reflectiveness", &reflectiveness, 0.0f, 1.0f);
                    ImGui::SliderFloat("Shininess", &shininess, 0.0f, 100.0f);
                    ImGui::SliderFloat("Ambient", &ambient, 0.0f, 1.0f);
                    ImGui::SliderFloat("Transparency", &transparency, 0.0f, 1.0f);
                    ImGui::SliderFloat("Refractive Index", &refractive_index, 1.0f, 10.0f);

                    shape->get_material().set_color(color_vec).set_specular(specular).set_diffuse(diffuse).set_reflectiveness(reflectiveness).set_shininess(shininess).set_ambient(ambient).set_transparency(transparency).set_refractive_index(refractive_index);
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

        ImGui::EndGroup();

        ImGui::TreePop(); // Details
    }
}

#endif