//
// Created by Dainius Rynkevicius on 27/04/2026.
//

#ifndef VOXELENGINE_CHUNK_H
#define VOXELENGINE_CHUNK_H
#include <array>
#include <algorithm>

#include "glm/glm.hpp"


namespace World {
    class Chunk {
    public:
        static constexpr unsigned int CHUNK_SIZE = 8;

        Chunk() = default;

        void SetBlock(glm::ivec3 localPos, uint8_t block);

        size_t GetIndex(glm::ivec3 localPos);

        uint8_t GetBlock(glm::ivec3 localPos);

        [[nodiscard]] bool Empty() const {
            return empty;
        }

    private:
        std::array<uint8_t, CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE> blocks{};
        bool empty = true;
    };
}

#endif //VOXELENGINE_CHUNK_H
