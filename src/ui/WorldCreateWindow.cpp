//
// Created by daini on 05/05/2026.
//

#include "WorldCreateWindow.h"

#include "imgui.h"
#include <glm/gtc/type_ptr.hpp>



namespace Ui {
    void WorldCreateWindow::Draw(double deltaTime) {
        if (open) {
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
                    case 1:
                        application.GenerateWorld(std::make_unique<World::Generators::HillGenerator>(seed, hill),
                                                  level_size);
                        break;
                    case 2:
                        application.GenerateWorld(std::make_unique<World::Generators::CaveGenerator>(seed, cave),
                                                  level_size);
                        break;
                    default:
                        spdlog::error("Unknown generator selected.");
                        break;
                }
            }
            ImGui::Text("Warning: Clicking generate will freeze the window while generating.");

            ImGui::End();
        }
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
        }else if (generator_selected == 1) {
            float raw_freq = hill.frequency;
            float raw_amp = hill.amplitude;
            int raw_seed = seed;

            ImGui::InputInt("Seed", &raw_seed);
            ImGui::InputFloat("Frequency", &raw_freq, 0.1f, 0.2f);
            ImGui::InputFloat("Amplitude", &raw_amp, 1.f, 5.f);
            ImGui::InputInt("Base Height", &hill.base_height);

            hill.amplitude = std::clamp(raw_amp, 0.f, 200.f);
            hill.frequency = std::clamp(raw_freq, 0.001f, 3.0f);
            seed = std::clamp(raw_seed, 0, INT_MAX);
        } else if (generator_selected == 2) {
            float raw_freq = cave.frequency;
            float raw_threshold = cave.threshold;
            int raw_seed = seed;

            ImGui::InputInt("Seed", &raw_seed);
            ImGui::InputFloat("Frequency", &raw_freq, 0.1f, 0.2f);
            ImGui::InputFloat("Threshold", &raw_threshold, 0.01f, 0.1f);

            cave.frequency = std::clamp(raw_freq, 0.001f, 3.0f);
            cave.threshold = std::clamp(raw_threshold, 0.f, 1.0f);
            seed = std::clamp(raw_seed, 0, INT_MAX);
        }
        else {
            ImGui::Text("Unknown generator, no options available.");
        }
    }
} // Ui
