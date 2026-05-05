//
// Created by Dainius Rynkevicius on 27/04/2026.
//

#ifndef VOXELENGINE_WORLD_H
#define VOXELENGINE_WORLD_H
#include <memory>
#include <queue>

#define GLM_ENABLE_EXPERIMENTAL
#include <unordered_set>
#include <glm/gtx/hash.hpp>

#include "generators/Generator.h"
#include "spdlog/spdlog.h"

namespace World {
    class Level {
    public:
        Level(std::unique_ptr<Generators::Generator> &&generator);

        void GenerateChunk(glm::ivec3 chunk_pos);

        Chunk *GetChunk(glm::ivec3 chunk_pos) {
            const auto it = chunks.find(chunk_pos);
            if (it != chunks.end()) {
                return &it->second;
            }
            return nullptr;
        }

        void MarkDirty(glm::ivec3 pos) {
            dirty.emplace(pos);
        }

        std::optional<glm::ivec3> PopDirtyChunk() {
            if (dirty.empty()) {
                return std::nullopt;
            }
            const auto it = dirty.begin();
            auto pos = *it;
            dirty.erase(it);
            return pos;
        }

        uint32_t GetBlock(const glm::ivec3 global_pos) {
            auto [chunk_pos, block_pos] = GetChunkCoordinates(global_pos);

            if (const auto chunk = GetChunk(chunk_pos)) {
                return chunk->GetBlock(block_pos);
            }

            return 0; // air
        }

        void SetBlock(glm::ivec3 global_pos, uint32_t block) {
            auto [chunk_pos, block_pos] = GetChunkCoordinates(global_pos);
            if (auto chunk = GetChunk(chunk_pos)) {
                chunk->SetBlock(block_pos, block);
                MarkDirty(chunk_pos);
                for (auto pos: NeighboringChunks(chunk_pos)) {
                    MarkDirty(pos);
                }
                return;
            }
            spdlog::warn("Attempted to blace block id {} at: {} {} {} in non existant chunk.", block, block_pos.x,
                         block_pos.y, block_pos.z);
            //TODO: throw exception
        }

        /// Returns chunk pos, local block pos
        static std::pair<glm::ivec3, glm::ivec3> GetChunkCoordinates(glm::ivec3 global_pos) {
            glm::ivec3 chunk = glm::floor(glm::vec3(global_pos) / glm::vec3(Chunk::CHUNK_SIZE));
            glm::ivec3 block = (global_pos % glm::ivec3(Chunk::CHUNK_SIZE) + glm::ivec3(Chunk::CHUNK_SIZE)) %
                               glm::ivec3(Chunk::CHUNK_SIZE);

            return {chunk, block};
        }

        static std::array<glm::ivec3, 6> NeighboringChunks(const glm::ivec3 chunk_pos) {
            return {
                glm::ivec3{chunk_pos + glm::ivec3{1, 0, 0}},
                glm::ivec3{chunk_pos + glm::ivec3{-1, 0, 0}},

                glm::ivec3{chunk_pos + glm::ivec3{0, 1, 0}},
                glm::ivec3{chunk_pos + glm::ivec3{0, -1, 0}},

                glm::ivec3{chunk_pos + glm::ivec3{0, 0, 1}},
                glm::ivec3{chunk_pos + glm::ivec3{0, 0, -1}},
            };
        }

        static glm::ivec3 GetGlobalPos(glm::ivec3 chunk_pos, glm::ivec3 local_pos) {
            assert((local_pos.x >= 0 && local_pos.x < Chunk::CHUNK_SIZE &&
                local_pos.y >= 0 && local_pos.y < Chunk::CHUNK_SIZE &&
                local_pos.z >= 0 && local_pos.z < Chunk::CHUNK_SIZE) && "Invalid local position");
            return chunk_pos * glm::ivec3(Chunk::CHUNK_SIZE) + local_pos;
        }

    private:
        std::unique_ptr<Generators::Generator> generator;

        std::unordered_map<glm::ivec3, Chunk> chunks;
        std::unordered_set<glm::ivec3> dirty;
    };
}
#endif //VOXELENGINE_WORLD_H
