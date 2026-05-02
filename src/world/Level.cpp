//
// Created by Dainius Rynkevicius on 27/04/2026.
//

#include "Level.h"

World::Level::Level(std::unique_ptr<Generators::Generator>&& generator) : generator(std::move(generator)) {
}

void World::Level::GenerateChunk(glm::ivec3 chunk_pos) {
    auto& chunk = chunks.emplace(chunk_pos, Chunk{}).first->second;
    generator->GenerateChunk(chunk_pos, chunk);
    dirty.emplace(chunk_pos);
}
