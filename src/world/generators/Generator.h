//
// Created by Dainius Rynkevicius on 01/05/2026.
//

#ifndef VOXELENGINE_GENERATOR_H
#define VOXELENGINE_GENERATOR_H
#include "../Chunk.h"
#include "glm/vec3.hpp"

namespace World::Generators {
    class Generator {
    public:
        virtual ~Generator() = default;
        virtual void GenerateChunk(glm::ivec3 chunk_pos, Chunk& chunk) = 0;
    };
}

#endif //VOXELENGINE_GENERATOR_H
