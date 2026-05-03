//
// Created by Dainius Rynkevicius on 01/05/2026.
//

#ifndef VOXELENGINE_CHUNKMESHER_H
#define VOXELENGINE_CHUNKMESHER_H
#include "BlockTextures.h"
#include "MeshData.h"
#include "../world/Level.h"
#include "glm/glm.hpp"
#include "webgpu/webgpu-raii.hpp"
#include "webgpu/webgpu.hpp"

namespace World::Blocks {
    class BlockRegistry;
}

namespace Render {
    class ChunkMesher {
    public:
        static MeshData GenerateMesh(glm::ivec3 chunk_pos, World::Level &level, World::Blocks::BlockRegistry &registry, BlockTextures &textures);

    private:
        static uint8_t SampleBlock(World::Chunk &chunk, const std::array<World::Chunk *, 6> &neighbors,
                                   glm::ivec3 block_pos);

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

        static constexpr float ATLAS_H = 3; // number of cubes in texture vertical
        static constexpr float ATLAS_W = 4; // number of cubes in texture horizontal

        static constexpr std::array<glm::ivec2, 6> atlas_uv_positions = {
            glm::ivec2{2, 1}, {0, 1}, {1, 0}, {1, 2}, {1, 1}, {3, 1}
        };

        static std::array<glm::vec2, 4> GenerateUvs(glm::ivec2 uv_pos) {
            constexpr float TILE_W = 1.0f / ATLAS_W;
            constexpr float TILE_H = 1.0f / ATLAS_H;

            float u0 = static_cast<float>(uv_pos.x) * TILE_W;
            float v0 = static_cast<float>(uv_pos.y) * TILE_H;
            float u1 = u0 + TILE_W;
            float v1 = v0 + TILE_H;

            return {
                glm::vec2{u0, v1},
                glm::vec2{u1, v1},
                glm::vec2{u1, v0},
                glm::vec2{u0, v0},
            };
        }

        static std::array<World::Chunk *, 6> GetNeighbors(const glm::ivec3 chunk_pos, World::Level &level) {
            std::array<World::Chunk *, 6> neighbors{};
            for (int i = 0; i < face_offsets.size(); ++i) {
                neighbors[i] = level.GetChunk(chunk_pos + face_offsets[i]);
            }
            return neighbors;
        }
    };
} // Render

#endif //VOXELENGINE_CHUNKMESHER_H
