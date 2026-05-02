//
// Created by Dainius Rynkevicius on 01/05/2026.
//

#ifndef VOXELENGINE_BLOCKREGISTRY_H
#define VOXELENGINE_BLOCKREGISTRY_H


#include <optional>
#include <vector>

#include "Block.h"

namespace World::Blocks {

    class BlockRegistry {
    public:
        BlockRegistry();
        std::optional<Block> Get(size_t id);

    private:
        std::vector<Block> blocks;
    };
}

#endif //VOXELENGINE_BLOCKREGISTRY_H
