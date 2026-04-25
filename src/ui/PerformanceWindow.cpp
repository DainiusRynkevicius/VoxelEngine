//
// Created by Dainius Rynkevicius on 25/04/2026.
//

#include "PerformanceWindow.h"

#include <format>

#include "imgui.h"

void Ui::PerformanceWindow::Draw(double deltaTime) {
    double fps = 1.0/deltaTime;
    double deltaMs = deltaTime*1000;

    ImGui::Begin("Performance");

    ImGui::Text("FPS: %.1f", fps);
    ImGui::Text("Delta time: %.3f ms", deltaMs);

    //TODO: add graph

    ImGui::End();
}
