//
// Created by Dainius Rynkevicius on 24/04/2026.
//

#include "Application.h"

Application::Application() : window({800, 600}, "Voxel Engine"), gpu_context(window), renderer(gpu_context) {
}

void Application::Run() {
    while (!glfwWindowShouldClose(window.Get())) {
        glfwPollEvents();
        renderer.Render(gpu_context);
    }
}
