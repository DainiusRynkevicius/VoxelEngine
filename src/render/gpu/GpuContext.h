//
// Created by Dainius Rynkevicius on 24/04/2026.
//

#ifndef VOXELENGINE_GPUCONTEXT_H
#define VOXELENGINE_GPUCONTEXT_H

#include <webgpu/webgpu-raii.hpp>

#include <optional>

#include "Frame.h"
#include "../../Window.h"

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

        wgpu::Device Device() { return *device; }
        wgpu::Queue Queue() { return *queue; }
        [[nodiscard]] wgpu::TextureFormat ColorFormat() const { return surface_configuration.format; };

        [[nodiscard]] glm::ivec2 SurfaceSize() const {
            return {surface_configuration.width, surface_configuration.height};
        }

        std::optional<Frame> StartFrame();

        void EndFrame();

        void Resize(int width, int height, GLFWwindow* window);
    };
}


#endif //VOXELENGINE_GPUCONTEXT_H
