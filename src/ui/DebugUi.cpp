//
// Created by Dainius Rynkevicius on 25/04/2026.
//

#include "DebugUi.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_wgpu.h"

namespace Ui {
    DebugUi::DebugUi(Render::Gpu::GpuContext& gpu, Window& window) {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

        ImGui::StyleColorsDark();

        float scale = ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor());

        ImGuiStyle& style = ImGui::GetStyle();
        style.ScaleAllSizes(scale);
        style.FontScaleDpi = scale;

        ImGui_ImplGlfw_InitForOther(window.Get(), true);

        ImGui_ImplWGPU_InitInfo init_info{};
        init_info.Device = gpu.Device();
        init_info.NumFramesInFlight = 3;
        init_info.RenderTargetFormat = gpu.ColorFormat();
        init_info.DepthStencilFormat = WGPUTextureFormat_Depth32Float;
        ImGui_ImplWGPU_Init(&init_info);
    }

    DebugUi::~DebugUi() {
        ImGui_ImplWGPU_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }


    void DebugUi::Update(double deltaTime) const {
        ImGui_ImplWGPU_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        for (auto& drawable: drawables) {
            drawable->Draw(deltaTime);
        }
    }

    void DebugUi::Render(wgpu::RenderPassEncoder pass) {
        ImGui::Render();
        ImGui_ImplWGPU_RenderDrawData(ImGui::GetDrawData(), pass);
    }

    void DebugUi::AddDrawable(std::unique_ptr<ImguiDrawable> drawable) {
        drawables.push_back(std::move(drawable));
    }
} // Ui