//
// Created by dainiusr on 5/3/26.
//

#ifndef VOXELENGINE_CHUNKBUFFERS_H
#define VOXELENGINE_CHUNKBUFFERS_H
#include "MeshData.h"
#include "webgpu/webgpu-raii.hpp"

namespace Render {

class ChunkMesh {
public:
    ChunkMesh(MeshData& data, wgpu::Device device, wgpu::Queue queue, wgpu::BindGroupLayout chunk_layout, glm::ivec3 chunk_pos);

    // no copy
    ChunkMesh(const ChunkMesh&) = delete;
    ChunkMesh& operator=(const ChunkMesh&) = delete;

    // allow move
    ChunkMesh(ChunkMesh&&) noexcept = default;
    ChunkMesh& operator=(ChunkMesh&&) noexcept = default;

    /// Pipeline must be bound before this
    void Draw(wgpu::RenderPassEncoder pass);

private:
    wgpu::raii::Buffer vertex;
    wgpu::raii::Buffer index;
    wgpu::raii::Buffer chunk_uniform;
    wgpu::raii::BindGroup chunk_uniform_group;

    uint32_t index_count;
};

} // Render

#endif //VOXELENGINE_CHUNKBUFFERS_H
