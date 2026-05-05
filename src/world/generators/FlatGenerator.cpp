//
// Created by Dainius Rynkevicius on 01/05/2026.
//

#include "FlatGenerator.h"


namespace World::Generators {
    void FlatGenerator::GenerateChunk(glm::ivec3 chunk_pos, Chunk& chunk) {
        int chunk_bottom_y = chunk_pos.y * Chunk::CHUNK_SIZE + 1;

        // Chunk does not have the height required
        if (chunk_bottom_y >= options.height) {
            return;
        }

        for (int x = 0; x < Chunk::CHUNK_SIZE; ++x) {
            for (int z = 0; z < Chunk::CHUNK_SIZE; ++z) {
                for (int y = 0; y < Chunk::CHUNK_SIZE; ++y) {
                    if (chunk_bottom_y + y <options. height) {
                        chunk.SetBlock({x, y, z}, options.block_id);
                    }
                }
            }
        }
    }
}
