//
// Created by dainiusr on 5/6/26.
//

#ifndef VOXELENGINE_NOISEGENERATOR_H
#define VOXELENGINE_NOISEGENERATOR_H
#include "Generator.h"

namespace World::Generators {
    class NoiseGenerator : public Generator{
    public:
        explicit NoiseGenerator(const uint32_t seed) : seed(seed){}

    protected:
        uint32_t seed;
    };
}

#endif //VOXELENGINE_NOISEGENERATOR_H
