//
// Created by dainiusr on 5/6/26.
//

#ifndef VOXELENGINE_CAVEGENERATOR_H
#define VOXELENGINE_CAVEGENERATOR_H
#include "NoiseGenerator.h"
#include "FastNoise/FastNoise.h"

namespace World::Generators {
    class CaveGenerator : public NoiseGenerator{
    public:
        struct Options {
            float frequency = 2.6f;
            float threshold = 0.48f;
        };
        void GenerateChunk(glm::ivec3 chunk_pos, Chunk &chunk) override;
        CaveGenerator(uint32_t seed, Options options = {2.6f, 0.48f});

    private:
        Options options;
        FastNoise::SmartNode<FastNoise::FractalFBm> noise;
    };
}

#endif //VOXELENGINE_CAVEGENERATOR_H
