//
// Created by Dainius Rynkevicius on 25/04/2026.
//

#ifndef VOXELENGINE_PIPELINE_H
#define VOXELENGINE_PIPELINE_H
#include <filesystem>

#include "GpuContext.h"


namespace Render::Gpu {

class Pipeline {
public:
    Pipeline(GpuContext& ctx);

    // No copy
    Pipeline(const Pipeline&) = delete;
    Pipeline& operator=(const Pipeline&) = delete;

    // allow move
    Pipeline(Pipeline&&)  noexcept = default;
    Pipeline& operator=(Pipeline&&)  noexcept = default;

    wgpu::RenderPipeline Get() {
        return *pipeline;
    }



private:
    wgpu::raii::PipelineLayout layout;
    wgpu::raii::RenderPipeline pipeline;

    wgpu::raii::ShaderModule LoadShaderModule(std::filesystem::path path, wgpu::Device device);
};

} // Render::Gpu

#endif //VOXELENGINE_PIPELINE_H
