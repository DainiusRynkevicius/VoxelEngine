//
// Created by Dainius Rynkevicius on 01/05/2026.
//

#include "BlockRegistry.h"


namespace World::Blocks {
    BlockRegistry::BlockRegistry() {
        blocks.emplace_back("Air", "", false); // Must always exist
        blocks.emplace_back("Dice", "textures/dice.png", true);
    }

    std::optional<Block> BlockRegistry::Get(size_t id) {
        if (id >= blocks.size()) {
            return {};
        }

        return blocks[id];
    }
}
