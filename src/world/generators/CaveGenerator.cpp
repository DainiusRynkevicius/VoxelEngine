//
// Created by dainiusr on 5/6/26.
//

#include "CaveGenerator.h"

namespace World {
    namespace Generators {
        void CaveGenerator::GenerateChunk(glm::ivec3 chunk_pos, Chunk &chunk) {
            constexpr auto S = Chunk::CHUNK_SIZE;
            glm::ivec3 world_pos = chunk_pos * glm::ivec3{S};

            std::array<float, S * S * S> heightmap{};
            noise->GenUniformGrid3D(heightmap.data(), world_pos.x * options.frequency, world_pos.y * options.frequency, world_pos.z * options.frequency, S, S, S, options.frequency, options.frequency, options.frequency, seed);

            for (int x = 0; x < S; ++x) {
                for (int y = 0; y < S; ++y) {
                    for (int z = 0; z < S; ++z) {
                        float s = (heightmap[(z * S + y) * S + x] + 1.f) * 0.5f;
                        if (s >= options.threshold) {
                            chunk.SetBlock({x,y,z}, 5);
                        }
                    }
                }
            }
        }

        CaveGenerator::CaveGenerator(uint32_t seed, Options options) : NoiseGenerator(seed), options(options){
            auto src = FastNoise::New<FastNoise::Simplex>();
            noise = FastNoise::New<FastNoise::FractalFBm>();
            noise->SetSource(src);
            noise->SetWeightedStrength(0.3f);
            noise->SetOctaveCount(2);
            noise->SetLacunarity(2.f);
        }
    } // Generators
} // World