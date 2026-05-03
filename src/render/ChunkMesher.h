//
// Created by Dainius Rynkevicius on 01/05/2026.
//

#ifndef VOXELENGINE_CHUNKMESHER_H
#define VOXELENGINE_CHUNKMESHER_H
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
        static MeshData GenerateMesh(glm::ivec3 chunk_pos, World::Level &level, World::Blocks::BlockRegistry &registry);
    private:
        static uint8_t SampleBlock(World::Chunk& chunk, const std::array<World::Chunk*, 6>& neighbors, glm::ivec3 block_pos);

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

        static std::array<World::Chunk*, 6> GetNeighbors(const glm::ivec3 chunk_pos, World::Level& level) {
            std::array<World::Chunk*, 6> neighbors{};
            for (int i = 0; i < face_offsets.size(); ++i) {
                neighbors[i] = level.GetChunk(chunk_pos + face_offsets[i]);
            }
            return neighbors;
        }
    };
} // Render

#endif //VOXELENGINE_CHUNKMESHER_H
