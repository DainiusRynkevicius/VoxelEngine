//
// Created by Dainius Rynkevicius on 27/04/2026.
//

#include "Chunk.h"

void World::Chunk::SetBlock(glm::ivec3 localPos, uint8_t block) {
    blocks[GetIndex(localPos)] = block;
    if (block!=0) {
        empty = false;
    }
}

size_t World::Chunk::GetIndex(glm::ivec3 localPos) {
    return localPos.x + localPos.y*CHUNK_SIZE + localPos.z*CHUNK_SIZE*CHUNK_SIZE;
}

uint8_t World::Chunk::GetBlock(glm::ivec3 localPos) {
    return blocks[GetIndex(localPos)];
}
