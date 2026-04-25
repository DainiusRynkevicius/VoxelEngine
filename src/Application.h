//
// Created by Dainius Rynkevicius on 24/04/2026.
//

#ifndef VOXELENGINE_APPLICATION_H
#define VOXELENGINE_APPLICATION_H
#include "render/Renderer.h"
#include "Window.h"
#include "render/gpu/GpuContext.h"


class Application {
private:
    Window window;
    Render::Gpu::GpuContext gpu_context;
    render::Renderer renderer;
    Ui::DebugUi imgui;

    double last_time;

public:
    static Application* Get();
    Application();
    void Run();
    static void ResizeCallback(GLFWwindow* window, int width, int height);
};



#endif //VOXELENGINE_APPLICATION_H
