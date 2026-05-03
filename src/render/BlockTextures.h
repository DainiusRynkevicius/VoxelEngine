//
// Created by dainiusr on 5/3/26.
//

#ifndef VOXELENGINE_BLOCKTEXTURES_H
#define VOXELENGINE_BLOCKTEXTURES_H
#include "../world/blocks/BlockRegistry.h"
#include "glm/vec2.hpp"
#include "gpu/GpuContext.h"
#include "webgpu/webgpu-raii.hpp"

namespace Render {

class BlockTextures {
public:
    static constexpr glm::ivec2 TEXTURE_SIZE = {128,96}; // cube map dimensions of 32x32 block texture

    // no copy
    BlockTextures(const BlockTextures&) = delete;
    BlockTextures& operator=(const BlockTextures&) = delete;

    // allow move
    BlockTextures(BlockTextures&&) noexcept = default;
    BlockTextures& operator=(BlockTextures&&) noexcept = default;

    BlockTextures(Gpu::GpuContext& ctx, World::Blocks::BlockRegistry& registry, wgpu::BindGroupLayout layout);

    uint32_t GetLayer(const std::string &texture_name);

    wgpu::BindGroup GetGroup() {
        return *group;
    }

private:
    wgpu::raii::Texture texture;
    wgpu::raii::TextureView view;
    wgpu::raii::Sampler sampler;

    wgpu::raii::BindGroup group;

    std::unordered_map<std::string, uint32_t> layers;
};

} // Render

#endif //VOXELENGINE_BLOCKTEXTURES_H
