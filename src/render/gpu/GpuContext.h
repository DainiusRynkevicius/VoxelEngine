//
// Created by Dainius Rynkevicius on 24/04/2026.
//

#ifndef VOXELENGINE_GPUCONTEXT_H
#define VOXELENGINE_GPUCONTEXT_H

#include <webgpu/webgpu-raii.hpp>

#include <optional>

#include "Frame.h"
#include "../Window.h"

namespace Render::Gpu {
    class GpuContext {
    private:
        wgpu::raii::Instance instance;
        wgpu::raii::Surface surface;
        wgpu::raii::Adapter adapter;
        wgpu::raii::Device device;
        wgpu::raii::Queue queue;

        wgpu::SurfaceConfiguration surface_configuration;
    public:
        explicit GpuContext(Window& window);

        wgpu::Device Device(){return *device;}
        wgpu::Queue Queue(){return *queue;}

        std::optional<Frame> StartFrame();
        void EndFrame();
    };
}




#endif //VOXELENGINE_GPUCONTEXT_H
