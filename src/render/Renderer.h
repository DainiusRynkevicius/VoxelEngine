//
// Created by Dainius Rynkevicius on 24/04/2026.
//

#ifndef VOXELENGINE_RENDERER_H
#define VOXELENGINE_RENDERER_H
#include "gpu/GpuContext.h"
#include "gpu/Pipeline.h"
#include <array>

#include "Camera.h"
#include "../ui/DebugUi.h"

namespace render {

class Renderer {
public:
    struct Vertex {
        glm::vec3 position;
        glm::vec3 color;

        static wgpu::VertexBufferLayout GetLayout() {
            wgpu::VertexBufferLayout layout{};
            layout.arrayStride = sizeof(Vertex);
            layout.stepMode = wgpu::VertexStepMode::Vertex;
            static std::array<WGPUVertexAttribute, 2> attributes;
            attributes[0].format = wgpu::VertexFormat::Float32x3;
            attributes[0].offset = offsetof(Vertex, position);
            attributes[0].shaderLocation = 0;

            attributes[1].format = wgpu::VertexFormat::Float32x3;
            attributes[1].offset = offsetof(Vertex, color);
            attributes[1].shaderLocation = 1;

            layout.attributeCount = attributes.size();
            layout.attributes = attributes.data();
            return layout;
        }
    };
    Renderer(Render::Gpu::GpuContext& ctx);

    void Render(Render::Gpu::GpuContext& ctx, Ui::DebugUi& imgui, Render::Camera& camera);

private:
    Render::Gpu::Pipeline pipeline;
    wgpu::raii::Buffer vertex_buffer;
    wgpu::raii::Buffer frame_uniform;
    wgpu::raii::BindGroup frame_uniform_group;

    std::vector<Vertex> vertices = {
        {{0, 1, 0},{1,0,0}},
        {{-1, -1, 0}, {0, 1, 0}},
        {{1,-1, 0}, {0, 0, 1}},
    };
};

} // render

#endif //VOXELENGINE_RENDERER_H
