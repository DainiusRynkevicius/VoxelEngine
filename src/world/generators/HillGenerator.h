//
// Created by dainiusr on 5/6/26.
//

#ifndef VOXELENGINE_HILLGENERATOR_H
#define VOXELENGINE_HILLGENERATOR_H
#include "NoiseGenerator.h"
#include "FastNoise/FastNoise.h"

namespace World::Generators {

    class HillGenerator : public NoiseGenerator {
    public:
        struct Options {
            float frequency = 0.5f;
            int base_height = 2;
            float amplitude = 27.f;
        };
        explicit HillGenerator(uint32_t seed, Options options = {0.5f, 2, 27.f});


        void GenerateChunk(glm::ivec3 chunk_pos, Chunk &chunk) override;

    private:
        Options options;
        FastNoise::SmartNode<FastNoise::FractalFBm> noise;
    };

}

#endif //VOXELENGINE_HILLGENERATOR_H
