//
// Created by dainiusr on 5/3/26.
//

#include "ChunkMesh.h"

#include "ChunkUniform.h"
#include "../world/Chunk.h"
#include "glm/ext/matrix_transform.hpp"
#include <format>


Render::ChunkMesh::ChunkMesh(MeshData &data, wgpu::Device device, wgpu::Queue queue, wgpu::BindGroupLayout chunk_layout, glm::ivec3 chunk_pos) {
    wgpu::BufferDescriptor v_desc{};
        v_desc.size = sizeof(ChunkVertex) * data.vertices.size();
        v_desc.mappedAtCreation = false;
        v_desc.usage = wgpu::BufferUsage::Vertex | wgpu::BufferUsage::CopyDst;
        v_desc.label = wgpu::StringView(std::format("Chunk [{} {} {}] vertex buffer", chunk_pos.x, chunk_pos.y,
                                                    chunk_pos.z));
        vertex = device.createBuffer(v_desc);
        queue.writeBuffer(*vertex, 0, data.vertices.data(), v_desc.size);

        wgpu::BufferDescriptor i_desc{};
        i_desc.size = sizeof(uint32_t) * data.indices.size();
        i_desc.label = wgpu::StringView(std::format("Chunk [{} {} {}] index buffer", chunk_pos.x, chunk_pos.y,
                                                    chunk_pos.z));
        i_desc.mappedAtCreation = false;
        i_desc.usage = wgpu::BufferUsage::Index | wgpu::BufferUsage::CopyDst;

        index = device.createBuffer(i_desc);
        queue.writeBuffer(*index, 0, data.indices.data(), i_desc.size);

        glm::vec3 world_pos = static_cast<glm::vec3>(chunk_pos) * glm::vec3(World::Chunk::CHUNK_SIZE);

        ChunkUniform uniform{glm::translate(glm::mat4{1.0f}, world_pos)};
        wgpu::BufferDescriptor u_desc{};
        u_desc.size = sizeof(ChunkUniform);
        u_desc.label = wgpu::StringView(std::format("Chunk [{} {} {}] chunk uniform buffer", chunk_pos.x, chunk_pos.y,
                                                    chunk_pos.z));
        u_desc.mappedAtCreation = false;
        u_desc.usage = wgpu::BufferUsage::Uniform | wgpu::BufferUsage::CopyDst;

        chunk_uniform = device.createBuffer(u_desc);
        queue.writeBuffer(*chunk_uniform, 0, &uniform, u_desc.size);

        index_count = data.indices.size();


        wgpu::BindGroupEntry entry{};
        entry.size = chunk_uniform->getSize();
        entry.binding = 0;
        entry.buffer = *chunk_uniform;
        entry.offset = 0;

        wgpu::BindGroupDescriptor desc{};
        desc.layout = chunk_layout;
        desc.entryCount = 1;
        desc.entries = &entry;

        chunk_uniform_group = device.createBindGroup(desc);
}


void Render::ChunkMesh::Draw(wgpu::RenderPassEncoder pass) {
    if (index_count == 0) {
        return;
    }
    pass.setVertexBuffer(0, *vertex, 0, vertex->getSize());
    pass.setIndexBuffer(*index, wgpu::IndexFormat::Uint32, 0, index->getSize());
    pass.setBindGroup(1, *chunk_uniform_group, 0, nullptr);
    pass.drawIndexed(index_count, 1, 0, 0, 0);
}
