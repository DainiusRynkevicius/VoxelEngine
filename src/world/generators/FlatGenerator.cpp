//
// Created by Dainius Rynkevicius on 01/05/2026.
//

#include "FlatGenerator.h"


namespace World::Generators {
    void FlatGenerator::GenerateChunk(glm::ivec3 chunk_pos, Chunk& chunk) {
        int chunk_bottom_y = chunk_pos.y * Chunk::CHUNK_SIZE;

        // Chunk does not have the height required
        if (chunk_bottom_y >= height) {
            return;
        }

        auto local_y = height - chunk_bottom_y;

        for (int i = 0; i < Chunk::CHUNK_SIZE; ++i) {
            for (int j = 0; j < Chunk::CHUNK_SIZE; ++j) {
                //TODO: add option for block
                chunk.SetBlock({i, local_y, j}, 1);
            }
        }
    }
}
