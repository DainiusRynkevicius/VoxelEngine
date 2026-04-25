//
// Created by Dainius Rynkevicius on 24/04/2026.
//

#include "Application.h"

#include "spdlog/spdlog.h"
#include "ui/DemoWindow.h"
#include "ui/PerformanceWindow.h"

static Application* instance;

Application* Application::Get() {
    assert(instance && "Application not created.");
    return instance;
}

Application::Application() : window({800, 600}, "Voxel Engine"), gpu_context(window), renderer(gpu_context),
                             imgui(gpu_context, window), camera() {
    instance = this;
    glfwSetFramebufferSizeCallback(window.Get(), ResizeCallback);
    imgui.AddDrawable<Ui::DemoWindow>();
    imgui.AddDrawable<Ui::PerformanceWindow>();

    last_time = glfwGetTime();
}

void Application::Run() {
    while (!glfwWindowShouldClose(window.Get())) {
        glfwPollEvents();

        double now = glfwGetTime();
        double delta = now - last_time;
        last_time = now;


        imgui.Update(delta);

        renderer.Render(gpu_context, imgui, camera);
    }
}

void Application::ResizeCallback(GLFWwindow* window, int width, int height) {
    spdlog::info("Resizing window...");
    Get()->gpu_context.Resize(width, height, window);
}
