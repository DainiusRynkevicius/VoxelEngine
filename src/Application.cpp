//
// Created by Dainius Rynkevicius on 24/04/2026.
//

#include "Application.h"

#include "spdlog/spdlog.h"
#include "ui/DemoWindow.h"
#include "ui/PerformanceWindow.h"
#include "world/generators/FlatGenerator.h"

static Application* instance;

Application* Application::Get() {
    assert(instance && "Application not created.");
    return instance;
}

Application::Application() : window({800, 600}, "Voxel Engine"), gpu_context(window), renderer(gpu_context,registry),
                             imgui(gpu_context, window), controller(camera),
                             level(std::make_unique<World::Generators::FlatGenerator>(8, 2)) {
    instance = this;
    glfwSetFramebufferSizeCallback(window.Get(), ResizeCallback);
    imgui.AddDrawable<Ui::DemoWindow>();
    imgui.AddDrawable<Ui::PerformanceWindow>();

    last_time = glfwGetTime();

    //TODO: change to dynamic

    // generate chunks
    constexpr int LEVEL_SIZE = 8;
    for (int x = -LEVEL_SIZE; x < LEVEL_SIZE; ++x) {
        for (int y = -LEVEL_SIZE; y < LEVEL_SIZE; ++y) {
            for (int z = -LEVEL_SIZE; z < LEVEL_SIZE; ++z) {
                level.GenerateChunk({x, y, z});
            }
        }
    }
}

void Application::Run() {
    while (!glfwWindowShouldClose(window.Get())) {
        glfwPollEvents();

        double now = glfwGetTime();
        double delta = now - last_time;
        last_time = now;

        controller.HandleInput(delta, window.Get());

        imgui.Update(delta);

        renderer.Render(gpu_context, imgui, camera, registry, level);
    }
}

void Application::ResizeCallback(GLFWwindow* window, int width, int height) {
    spdlog::info("Resizing window...");
    Get()->gpu_context.Resize(width, height, window);
    Get()->renderer.CreateDepthTexture(Get()->gpu_context);
}
