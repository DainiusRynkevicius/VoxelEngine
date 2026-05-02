//
// Created by Dainius Rynkevicius on 01/05/2026.
//

#ifndef VOXELENGINE_CHUNKMESHER_H
#define VOXELENGINE_CHUNKMESHER_H
#include "../world/Level.h"
#include "glm/glm.hpp"
#include "webgpu/webgpu-raii.hpp"
#include "webgpu/webgpu.hpp"

namespace World::Blocks {
    //TODO: split class into two: mesher and the one who manages the gpu resources
    class BlockRegistry;
}

namespace Render {
    class ChunkMesher {
    public:
        struct Vertex {
            glm::vec3 position;
            glm::vec2 uv;
            uint32_t texture_layer;

            static wgpu::VertexBufferLayout GetLayout() {
                wgpu::VertexBufferLayout layout{};
                layout.arrayStride = sizeof(Vertex);
                layout.stepMode = wgpu::VertexStepMode::Vertex;
                static std::array<WGPUVertexAttribute, 3> attributes;
                attributes[0].format = wgpu::VertexFormat::Float32x3;
                attributes[0].offset = offsetof(Vertex, position);
                attributes[0].shaderLocation = 0;

                attributes[1].format = wgpu::VertexFormat::Float32x2;
                attributes[1].offset = offsetof(Vertex, uv);
                attributes[1].shaderLocation = 1;

                attributes[2].format = wgpu::VertexFormat::Uint32;
                attributes[2].offset = offsetof(Vertex, texture_layer);
                attributes[2].shaderLocation = 2;

                layout.attributeCount = attributes.size();
                layout.attributes = attributes.data();
                return layout;
            }
        };

        struct ChunkBuffers {
            wgpu::raii::Buffer vertex;
            wgpu::raii::Buffer index;
            wgpu::raii::Buffer chunk_uniform;
            wgpu::raii::BindGroup chunk_uniform_group;

            uint32_t index_count;
        };

        void GenerateMesh(glm::ivec3 chunk_pos, World::Level& level, World::Blocks::BlockRegistry& registry,
                          wgpu::Device device, wgpu::Queue
                          queue, wgpu::BindGroupLayout chunk_uniform);

        std::unordered_map<glm::ivec3, ChunkBuffers>& GetChunkBuffers() {
            return chunk_buffers;
        }

    private:
        std::unordered_map<glm::ivec3, ChunkBuffers> chunk_buffers;

        uint8_t SampleBlock(World::Chunk& chunk, const std::array<World::Chunk*, 6>& neighbors, glm::ivec3 block_pos);

        static constexpr std::array<glm::ivec3, 6> face_offsets = {
            glm::ivec3{1, 0, 0},
            glm::ivec3{-1, 0, 0},
            glm::ivec3{0, 1, 0},
            glm::ivec3{0, -1, 0},
            glm::ivec3{0, 0, 1},
            glm::ivec3{0, 0, -1},
        };

        static constexpr std::array<std::array<glm::vec3, 4>, 6> face_corners = {
            std::array<glm::vec3, 4>{glm::vec3{1, 0, 0}, {1, 1, 0}, {1, 1, 1}, {1, 0, 1}},
            std::array<glm::vec3, 4>{glm::vec3{0, 0, 1}, {0, 1, 1}, {0, 1, 0}, {0, 0, 0}},
            std::array<glm::vec3, 4>{glm::vec3{0, 1, 0}, {0, 1, 1}, {1, 1, 1}, {1, 1, 0}},
            std::array<glm::vec3, 4>{glm::vec3{0, 0, 0}, {1, 0, 0}, {1, 0, 1}, {0, 0, 1}},
            std::array<glm::vec3, 4>{glm::vec3{0, 0, 1}, {1, 0, 1}, {1, 1, 1}, {0, 1, 1}},
            std::array<glm::vec3, 4>{glm::vec3{1, 0, 0}, {0, 0, 0}, {0, 1, 0}, {1, 1, 0}}
        };

        std::array<World::Chunk*, 6> GetNeighbors(const glm::ivec3 chunk_pos, World::Level& level) {
            std::array<World::Chunk*, 6> neighbors{};
            for (int i = 0; i < face_offsets.size(); ++i) {
                neighbors[i] = level.GetChunk(chunk_pos + face_offsets[i]);
            }
            return neighbors;
        }
    };
} // Render

#endif //VOXELENGINE_CHUNKMESHER_H
