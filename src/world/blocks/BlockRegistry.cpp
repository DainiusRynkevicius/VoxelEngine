//
// Created by Dainius Rynkevicius on 01/05/2026.
//

#include "BlockRegistry.h"


namespace World::Blocks {
    BlockRegistry::BlockRegistry() {
        blocks.emplace_back("Air", "", false); // Must always exist
        blocks.emplace_back("Template", "assets/textures/template.png", true);
        blocks.emplace_back("Dice", "assets/textures/dice.png", true);

    }

    std::optional<Block> BlockRegistry::Get(size_t id) {
        if (id >= blocks.size()) {
            return {};
        }

        return blocks[id];
    }

    std::vector<std::string> BlockRegistry::TextureNames() {
        std::vector<std::string> res;
        res.reserve(blocks.size() - 1);

        for (size_t i = 1; i < blocks.size(); ++i) { // skip air
            res.emplace_back(blocks[i].texture_name);
        }

        return res;
    }
}
