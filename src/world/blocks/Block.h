//
// Created by Dainius Rynkevicius on 29/04/2026.
//

#ifndef VOXELENGINE_BLOCK_H
#define VOXELENGINE_BLOCK_H
#include <string>


namespace World::Blocks{
    struct Block {
        std::string name;
        std::string texture_name;
        bool opaque;
    };
}
#endif //VOXELENGINE_BLOCK_H
