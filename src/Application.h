//
// Created by Dainius Rynkevicius on 24/04/2026.
//

#ifndef VOXELENGINE_APPLICATION_H
#define VOXELENGINE_APPLICATION_H
#include "render/Renderer.h"
#include "render/Window.h"
#include "render/gpu/GpuContext.h"


class Application {
private:
    Render::Window window;
    Render::Gpu::GpuContext gpu_context;
    render::Renderer renderer;

public:
    Application();
    void Run();
};



#endif //VOXELENGINE_APPLICATION_H
