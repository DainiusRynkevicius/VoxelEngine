//
// Created by Dainius Rynkevicius on 01/05/2026.
//

#include "ChunkMesher.h"

#include "ChunkUniform.h"
#include "MeshData.h"
#include "../Application.h"
#include "glm/ext/matrix_transform.hpp"
#include "spdlog/spdlog.h"
#include "../world/blocks/Block.h"
#include "../world/blocks/BlockRegistry.h"

namespace Render {
    MeshData ChunkMesher::GenerateMesh(glm::ivec3 chunk_pos, World::Level &level,
                                       World::Blocks::BlockRegistry &registry, BlockTextures &textures) {
        auto chunk = level.GetChunk(chunk_pos);
        if (!chunk) {
            spdlog::error("Received invalid chunk meshing request. Pos x: {}, y: {}, z: {}.", chunk_pos.x, chunk_pos.y,
                          chunk_pos.z);
            //TODO: custom exception
            throw std::runtime_error("Chunk does not exist");
        }
        auto neighbors = GetNeighbors(chunk_pos, level);

        MeshData data{};

        for (int x = 0; x < World::Chunk::CHUNK_SIZE; ++x) {
            for (int y = 0; y < World::Chunk::CHUNK_SIZE; ++y) {
                for (int z = 0; z < World::Chunk::CHUNK_SIZE; ++z) {
                    glm::ivec3 pos = {x, y, z};
                    uint8_t block_id = chunk->GetBlock(pos);
                    if (block_id == 0) {
                        // Air
                        continue;
                    }

                    for (int f = 0; f < 6; ++f) {
                        // TODO: maybe replace with level.GetBlock()?
                        auto sampled = SampleBlock(*chunk, neighbors, pos + face_offsets[f]);
                        if (registry.Get(sampled)->opaque)
                            continue;


                        uint8_t layer = textures.GetLayer(registry.Get(block_id)->texture_name);
                        auto base_count = data.vertices.size();


                        auto uv = GenerateUvs(atlas_uv_positions[f]);
                        for (int v = 0; v < 4; ++v) {
                            ChunkVertex vertex{};
                            vertex.texture_layer = layer;
                            //TODO: assign uv properly

                            vertex.uv = uv[v];
                            vertex.position = face_corners[f][v] + static_cast<glm::vec3>(pos);

                            data.vertices.push_back(vertex);
                        }

                        data.indices.push_back(base_count + 0);
                        data.indices.push_back(base_count + 1);
                        data.indices.push_back(base_count + 2);
                        data.indices.push_back(base_count + 0);
                        data.indices.push_back(base_count + 2);
                        data.indices.push_back(base_count + 3);
                    }
                }
            }
        }
        return data;
    }

    uint8_t ChunkMesher::SampleBlock(World::Chunk& chunk, const std::array<World::Chunk*, 6>& neighbors,
                                     glm::ivec3 block_pos) {
        auto chunk_size = World::Chunk::CHUNK_SIZE;
        // Check if block is in bounds of chunk
        if (block_pos.x >= 0 && block_pos.x < chunk_size &&
            block_pos.y >= 0 && block_pos.y < chunk_size &&
            block_pos.z >= 0 && block_pos.z < chunk_size) {
            return chunk.GetBlock(block_pos);
        }

        //TODO: place an assert if block pos is valid

        // If not return the block in neighbor if exists
        int neighborId;

        if (block_pos.x >= chunk_size) {
            neighborId = 0;
        } else if (block_pos.x < 0) {
            neighborId = 1;
        } else if (block_pos.y >= chunk_size) {
            neighborId = 2;
        } else if (block_pos.y < 0) {
            neighborId = 3;
        } else if (block_pos.z >= chunk_size) {
            neighborId = 4;
        } else  {
            neighborId = 5;
        }

        if (auto neighbor = neighbors[neighborId]) {
            const auto local_pos = (block_pos + glm::ivec3(chunk_size)) % glm::ivec3(chunk_size);
            return neighbor->GetBlock(local_pos);
        }

        return 0; // Air
    }
} // Render
