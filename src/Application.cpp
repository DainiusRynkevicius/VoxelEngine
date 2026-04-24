//
// Created by Dainius Rynkevicius on 24/04/2026.
//

#include "Application.h"

#include "spdlog/spdlog.h"

static Application* instance;

Application* Application::Get() {
    assert(instance && "Application not created.");
    return instance;
}

Application::Application() : window({800, 600}, "Voxel Engine"), gpu_context(window), renderer(gpu_context) {
    instance = this;
    glfwSetFramebufferSizeCallback(window.Get(), ResizeCallback);
}

void Application::Run() {
    while (!glfwWindowShouldClose(window.Get())) {
        glfwPollEvents();
        renderer.Render(gpu_context);
    }
}

void Application::ResizeCallback(GLFWwindow* window, int width, int height) {
    spdlog::info("Resizing window...");
    Get()->gpu_context.Resize(width, height, window);
}
