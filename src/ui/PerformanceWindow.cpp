//
// Created by Dainius Rynkevicius on 25/04/2026.
//

#include "PerformanceWindow.h"

#include <format>
#include <numeric>

#include "imgui.h"

void Ui::PerformanceWindow::Draw(double deltaTime) {
    double deltaMs = deltaTime * 1000; // s -> ms

    if (open) {
        frame_times[offset] = deltaMs;
        offset += 1;
        offset %= SAMPLE_SIZE;
        float sum = static_cast<float>(std::accumulate(frame_times.begin(), frame_times.end(), 0));
        float average = sum / SAMPLE_SIZE;
        float average_fps = 1000.0f / average;
        ImGui::Begin("Performance", &open);
        ImGui::Text("FPS: %.1f", average_fps);
        ImGui::Text("Delta time: %.3f ms", average);

        ImGui::PlotLines("Frame Time (ms)", frame_times.data(), SAMPLE_SIZE);

        ImGui::End();
    }
}
