//
// Created by Dainius Rynkevicius on 01/05/2026.
//

#ifndef VOXELENGINE_FLATGENERATOR_H
#define VOXELENGINE_FLATGENERATOR_H
#include "Generator.h"


namespace World::Generators {
    class FlatGenerator : public Generator {
    public:
        struct Options {
            /// Global y level
            int height = 1;
            /// Block used to fill
            uint32_t block_id = 1;
        };

        FlatGenerator(Options options) : options(options) {
        }

        void GenerateChunk(glm::ivec3 chunk_pos, Chunk &chunk) override;

    private:
        Options options;
    };
}

#endif //VOXELENGINE_FLATGENERATOR_H
