//
// Created by daini on 05/05/2026.
//

#include "WorldCreateWindow.h"

#include "imgui.h"
#include <glm/gtc/type_ptr.hpp>



namespace Ui {
    void WorldCreateWindow::Draw(double deltaTime) {
        ImGui::Begin("World Creation", &open);

        GeneratorSelector();

        ImGui::InputInt3("World Size", glm::value_ptr(level_size));

        ImGui::Separator();

        GeneratorOptions();

        ImGui::Separator();

        if (ImGui::Button("Generate")) {
            switch (generator_selected) {
                case 0:
                    application.GenerateWorld(std::make_unique<World::Generators::FlatGenerator>(flat), level_size);
                    break;
                default:
                    spdlog::error("Unknown generator selected.");
                    break;
            }
        }
        ImGui::Text("Warning: Clicking generate will freeze the window while generating.");

        ImGui::End();
    }

    void WorldCreateWindow::GeneratorSelector() {
        if (generator_selected >= generators.size() && generators.size() > 0) {
            generator_selected = 0;
            spdlog::warn("Selected generator does not exist, resetting...");
        }
        if (generators.empty()) {
            spdlog::warn("No generators detected.");
            return;
        }
        auto preview = generators[generator_selected].c_str();
        if (ImGui::BeginCombo("Generator Selector", preview)) {
            for (int i = 0; i < generators.size(); ++i) {
                bool selected = generator_selected == i;

                if (ImGui::Selectable(generators[i].c_str(), selected)) {
                    generator_selected = i;
                }

                if (selected) {
                    ImGui::SetItemDefaultFocus();
                }
            }

            ImGui::EndCombo();
        }
    }

    void WorldCreateWindow::GeneratorOptions() {
        if (generator_selected == 0) {
            int raw_selected_block = flat.block_id;

            ImGui::InputInt("Select fill block", &raw_selected_block);
            ImGui::InputInt("Select fill height", &flat.height);

            raw_selected_block = std::clamp(raw_selected_block, 0, static_cast<int>(registry.BlockCount()) - 1);

            flat.block_id = raw_selected_block;
        } else {
            ImGui::Text("Unknown generator, no options available.");
        }
    }
} // Ui
