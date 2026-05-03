//
// Created by Dainius Rynkevicius on 24/04/2026.
//

#ifndef VOXELENGINE_APPLICATION_H
#define VOXELENGINE_APPLICATION_H
#include "CameraController.h"
#include "render/Renderer.h"
#include "Window.h"
#include "render/Camera.h"
#include "render/gpu/GpuContext.h"
#include "world/Level.h"
#include "world/blocks/BlockRegistry.h"


class Application {
private:
    World::Blocks::BlockRegistry registry;

    Window window;
    Render::Gpu::GpuContext gpu_context;
    Render::Renderer renderer;
    Ui::DebugUi imgui;
    Render::Camera camera{};
    CameraController controller;

    World::Level level;

    double last_time;

public:
    static Application* Get();
    Application();
    void Run();
    static void ResizeCallback(GLFWwindow* window, int width, int height);
};



#endif //VOXELENGINE_APPLICATION_H
