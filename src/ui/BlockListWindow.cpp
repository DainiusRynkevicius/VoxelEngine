//
// Created by Dainius Rynkevicius on 07/05/2026.
//

#include "BlockListWindow.h"

#include <algorithm>
#include <numeric>

#include "imgui.h"

namespace Ui {
    BlockListWindow::BlockListWindow(World::Blocks::BlockRegistry& registry) : registry(registry) {
    }

    void BlockListWindow::Draw(double deltaTime) {
        ImGui::Begin("Block List", &open);

        ImGui::LabelText("Block list:", "");

        if (ImGui::BeginTable("block_table", 4)) {
            std::vector<int> sorted{};
            sorted.resize(registry.BlockCount());

            std::iota(sorted.begin(), sorted.end(), 0);
            std::ranges::sort(sorted, [&](const int a, const int b) {
                if (registry.Get(a)->name < registry.Get(b)->name) {
                    return true;
                }
                return false;
            });

            ImGui::TableSetupColumn("ID");
            ImGui::TableSetupColumn("Name");
            ImGui::TableSetupColumn("Texture Name");
            ImGui::TableSetupColumn("Opaque");

            for (int i: sorted) {
                ImGui::TableNextColumn();
                auto block = registry.Get(i);
                auto id_str = std::to_string(i);
                ImGui::Text("%s", id_str.c_str());
                ImGui::TableNextColumn();
                ImGui::Text("%s", block->name.c_str());
                ImGui::TableNextColumn();
                ImGui::Text("%s", block->texture_name.c_str());
                ImGui::TableNextColumn();
                ImGui::BeginDisabled();
                ImGui::Checkbox("", &block->opaque);
                ImGui::EndDisabled();
            }

            ImGui::EndTable();
        }

        ImGui::End();
    }
} // Ui
