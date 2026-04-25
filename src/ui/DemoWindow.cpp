//
// Created by Dainius Rynkevicius on 25/04/2026.
//

#include "DemoWindow.h"

#include "imgui.h"

namespace Ui {
    void DemoWindow::Draw(double deltaTime) {
        ImGui::ShowDemoWindow(&open);
    }
} // Ui