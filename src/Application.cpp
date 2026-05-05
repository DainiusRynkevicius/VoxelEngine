//
// Created by Dainius Rynkevicius on 24/04/2026.
//

#include "Application.h"

#include "spdlog/spdlog.h"
#include "ui/DemoWindow.h"
#include "ui/PerformanceWindow.h"
#include "ui/WorldCreateWindow.h"
#include "world/generators/FlatGenerator.h"

static Application* instance;

Application* Application::Get() {
    assert(instance && "Application not created.");
    return instance;
}

Application::Application() : window({800, 600}, "Voxel Engine"), gpu_context(window), renderer(gpu_context,registry),
                             imgui(gpu_context, window), controller(camera){
    instance = this;
    glfwSetFramebufferSizeCallback(window.Get(), ResizeCallback);
    imgui.AddDrawable<Ui::DemoWindow>();
    imgui.AddDrawable<Ui::PerformanceWindow>();
    imgui.AddDrawable<Ui::WorldCreateWindow>(*this, registry);

    last_time = glfwGetTime();
}

void Application::Run() {
    while (!glfwWindowShouldClose(window.Get())) {
        glfwPollEvents();

        double now = glfwGetTime();
        double delta = now - last_time;
        last_time = now;

        controller.HandleInput(delta, window.Get());

        imgui.Update(delta);

        renderer.Render(gpu_context, imgui, camera, registry, level.get());
    }
}

void Application::ResizeCallback(GLFWwindow* window, int width, int height) {
    spdlog::info("Resizing window...");
    Get()->gpu_context.Resize(width, height, window);
    Get()->renderer.CreateDepthTexture(Get()->gpu_context);
}
