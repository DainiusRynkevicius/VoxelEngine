//
// Created by Dainius Rynkevicius on 01/05/2026.
//

#ifndef VOXELENGINE_FLATGENERATOR_H
#define VOXELENGINE_FLATGENERATOR_H
#include "Generator.h"


namespace World::Generators {

class FlatGenerator : public Generator{
public:
    FlatGenerator(unsigned int height = 1) : height(height){}
    void GenerateChunk(glm::ivec3 chunk_pos, Chunk& chunk) override;
private:
    /// Global y level
    unsigned int height;
};

}

#endif //VOXELENGINE_FLATGENERATOR_H
