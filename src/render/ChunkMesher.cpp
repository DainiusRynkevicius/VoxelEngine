//
// Created by Dainius Rynkevicius on 01/05/2026.
//

#include "ChunkMesher.h"

#include "ChunkUniform.h"
#include "../Application.h"
#include "glm/ext/matrix_transform.hpp"
#include "spdlog/spdlog.h"
#include "../world/blocks/Block.h"
#include "../world/blocks/BlockRegistry.h"

namespace Render {
    void ChunkMesher::GenerateMesh(glm::ivec3 chunk_pos, World::Level& level, World::Blocks::BlockRegistry& registry,
                                   wgpu::Device device, wgpu::Queue
                                   queue, wgpu::BindGroupLayout chunk_uniform) {
        auto chunk = level.GetChunk(chunk_pos);
        if (!chunk) {
            spdlog::error("Received invalid chunk meshing request. Pos x: {}, y: {}, z: {}.", chunk_pos.x, chunk_pos.y,
                          chunk_pos.z);
        }
        auto neighbors = GetNeighbors(chunk_pos, level);

        std::vector<Vertex> vertices{};
        std::vector<uint32_t> indices{};

        for (int x = 0; x < World::Chunk::CHUNK_SIZE; ++x) {
            for (int y = 0; y < World::Chunk::CHUNK_SIZE; ++y) {
                for (int z = 0; z < World::Chunk::CHUNK_SIZE; ++z) {
                    glm::ivec3 pos = {x, y, z};
                    uint8_t block_id = chunk->GetBlock(pos);
                    if (block_id == 0) {
                        // Air
                        continue;
                    }

                    for (int f = 0; f < 6; ++f) {
                        auto sampled = SampleBlock(*chunk, neighbors, pos + face_offsets[f]);
                        if (registry.Get(sampled)->opaque)
                            continue;

                        //TODO: Get correct layer
                        uint8_t layer = 0;
                        auto base_count = vertices.size();

                        for (int v = 0; v < 4; ++v) {
                            Vertex vertex{};
                            vertex.texture_layer = layer;
                            //TODO: assign uv properly
                            vertex.uv = glm::vec2{};
                            vertex.position = face_corners[f][v] + static_cast<glm::vec3>(pos);

                            vertices.push_back(vertex);
                        }

                        indices.push_back(base_count + 0);
                        indices.push_back(base_count + 1);
                        indices.push_back(base_count + 2);
                        indices.push_back(base_count + 0);
                        indices.push_back(base_count + 2);
                        indices.push_back(base_count + 3);
                    }
                }
            }
        }

        // Upload data

        wgpu::BufferDescriptor v_desc{};
        v_desc.size = sizeof(Vertex) * vertices.size();
        v_desc.mappedAtCreation = false;
        v_desc.usage = wgpu::BufferUsage::Vertex | wgpu::BufferUsage::CopyDst;
        v_desc.label = wgpu::StringView(std::format("Chunk [{} {} {}] vertex buffer", chunk_pos.x, chunk_pos.y,
                                                    chunk_pos.z));
        chunk_buffers[chunk_pos].vertex = device.createBuffer(v_desc);
        queue.writeBuffer(*chunk_buffers[chunk_pos].vertex, 0, vertices.data(), v_desc.size);

        wgpu::BufferDescriptor i_desc{};
        i_desc.size = sizeof(uint32_t) * indices.size();
        i_desc.label = wgpu::StringView(std::format("Chunk [{} {} {}] index buffer", chunk_pos.x, chunk_pos.y,
                                                    chunk_pos.z));
        i_desc.mappedAtCreation = false;
        i_desc.usage = wgpu::BufferUsage::Index | wgpu::BufferUsage::CopyDst;

        chunk_buffers[chunk_pos].index = device.createBuffer(i_desc);
        queue.writeBuffer(*chunk_buffers[chunk_pos].index, 0, indices.data(), i_desc.size);

        glm::vec3 world_pos = static_cast<glm::vec3>(chunk_pos) * glm::vec3(World::Chunk::CHUNK_SIZE);

        ChunkUniform uniform{glm::translate(glm::mat4{1.0f}, world_pos)};
        wgpu::BufferDescriptor u_desc{};
        u_desc.size = sizeof(ChunkUniform);
        u_desc.label = wgpu::StringView(std::format("Chunk [{} {} {}] chunk uniform buffer", chunk_pos.x, chunk_pos.y,
                                                    chunk_pos.z));
        u_desc.mappedAtCreation = false;
        u_desc.usage = wgpu::BufferUsage::Uniform | wgpu::BufferUsage::CopyDst;

        chunk_buffers[chunk_pos].chunk_uniform = device.createBuffer(u_desc);
        queue.writeBuffer(*chunk_buffers[chunk_pos].chunk_uniform, 0, &uniform, u_desc.size);

        chunk_buffers[chunk_pos].index_count = indices.size();


        wgpu::BindGroupEntry entry{};
        entry.size = chunk_buffers[chunk_pos].chunk_uniform->getSize();
        entry.binding = 0;
        entry.buffer = *chunk_buffers[chunk_pos].chunk_uniform;
        entry.offset = 0;

        wgpu::BindGroupDescriptor desc{};
        desc.layout = chunk_uniform;
        desc.entryCount = 1;
        desc.entries = &entry;

        chunk_buffers[chunk_pos].chunk_uniform_group = device.createBindGroup(desc);
    }

    uint8_t ChunkMesher::SampleBlock(World::Chunk& chunk, const std::array<World::Chunk*, 6>& neighbors,
                                     glm::ivec3 block_pos) {
        auto chunk_size = World::Chunk::CHUNK_SIZE;
        // Check if block is in bounds of chunk
        if (block_pos.x >= 0 && block_pos.x < chunk_size &&
            block_pos.y >= 0 && block_pos.y < chunk_size &&
            block_pos.z >= 0 && block_pos.z < chunk_size) {
            return chunk.GetBlock(block_pos);
        }

        //TODO: place an assert if block pos is valid

        // If not return the block in neighbor if exists
        int neighborId;

        if (block_pos.x >= chunk_size) {
            neighborId = 0;
        } else if (block_pos.x < 0) {
            neighborId = 1;
        } else if (block_pos.y >= chunk_size) {
            neighborId = 2;
        } else if (block_pos.y < 0) {
            neighborId = 3;
        } else if (block_pos.z >= chunk_size) {
            neighborId = 4;
        } else if (block_pos.z < 0) {
            neighborId = 5;
        }

        auto neighbor = neighbors[neighborId];
        if (neighbor) {
            auto local_pos = (block_pos + glm::ivec3(chunk_size)) % glm::ivec3(chunk_size);
            return neighbor->GetBlock(local_pos);
        }

        return 0; // Air
    }
} // Render
