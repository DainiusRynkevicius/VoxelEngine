//
// Created by dainiusr on 5/6/26.
//

#include "HillGenerator.h"

namespace World::Generators {
    HillGenerator::HillGenerator(uint32_t seed, Options options) : NoiseGenerator(seed), options(options) {
        auto src = FastNoise::New<FastNoise::Simplex>();
        noise = FastNoise::New<FastNoise::FractalFBm>();
        noise->SetSource(src);
        noise->SetOctaveCount(4);
        noise->SetLacunarity(2.0f);
        noise->SetGain(0.5f);
    }

    void HillGenerator::GenerateChunk(glm::ivec3 chunk_pos, Chunk &chunk) {
        constexpr auto S = Chunk::CHUNK_SIZE;
        glm::ivec3 world_pos = chunk_pos * glm::ivec3{S};

        std::array<float, S * S> heightmap{};
        noise->GenUniformGrid2D(heightmap.data(), world_pos.x * options.frequency, world_pos.z * options.frequency, S, S, options.frequency, options.frequency, seed);


        for (int z = 0; z < S; ++z) {
            for (int x = 0; x < S; ++x) {
                // [-1; 1] -> [0; 1]
                float c_noise = (heightmap[z * S + x] + 1.f) * 0.5f;
                int world_surface_y = options.base_height + static_cast<int>(c_noise * options.amplitude);

                for (int y = 0; y < S; ++y) {
                    int world_y = world_pos.y + y;

                    uint32_t block;

                    if (world_y > world_surface_y) {
                        block = 0;
                    }else if (world_y == world_surface_y) {
                        block = 4;
                    }else if (world_y >= world_surface_y - 3) {
                        block = 3;
                    }else {
                        block = 5;
                    }

                    chunk.SetBlock({x,y,z}, block);
                }
            }
        }
    }
}
