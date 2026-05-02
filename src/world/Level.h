//
// Created by Dainius Rynkevicius on 27/04/2026.
//

#ifndef VOXELENGINE_WORLD_H
#define VOXELENGINE_WORLD_H
#include <memory>

#define GLM_ENABLE_EXPERIMENTAL
#include <queue>
#include <glm/gtx/hash.hpp>

#include "generators/Generator.h"

namespace World {
    class Level {
    public:
        Level(std::unique_ptr<Generators::Generator>&& generator);

        void GenerateChunk(glm::ivec3 chunk_pos);

        Chunk* GetChunk(glm::ivec3 chunk_pos) {
            const auto it = chunks.find(chunk_pos);
            if (it != chunks.end()) {
                return &it->second;
            }
            return nullptr;
        }

        //TODO: remove
        std::unordered_map<glm::ivec3, Chunk>& GetChunks() {
            return chunks;
        }

    private:
        std::unique_ptr<Generators::Generator> generator;

        std::unordered_map<glm::ivec3, Chunk> chunks;
        std::queue<glm::ivec3> dirty;
    };
}
#endif //VOXELENGINE_WORLD_H
