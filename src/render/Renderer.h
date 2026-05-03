//
// Created by Dainius Rynkevicius on 24/04/2026.
//

#ifndef VOXELENGINE_RENDERER_H
#define VOXELENGINE_RENDERER_H
#include "gpu/GpuContext.h"
#include "gpu/Pipeline.h"
#include <array>

#include "Camera.h"
#include "ChunkMesh.h"
#include "ChunkMesher.h"
#include "../ui/DebugUi.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

namespace Render {
    class Renderer {
    public:
        Renderer(Render::Gpu::GpuContext& ctx);

        void Render(Render::Gpu::GpuContext& ctx, Ui::DebugUi& imgui, Render::Camera& camera,
                    World::Blocks::BlockRegistry& registry, World::
                    Level& level);

        void CreateDepthTexture(Render::Gpu::GpuContext& ctx);

    private:
        Gpu::Pipeline pipeline;

        wgpu::raii::Buffer frame_uniform;
        wgpu::raii::BindGroup frame_uniform_group;

        wgpu::raii::Texture depth_texture;
        wgpu::raii::TextureView depth_view;

        std::unordered_map<glm::ivec3, ChunkMesh> meshes;
    };
} // render

#endif //VOXELENGINE_RENDERER_H
