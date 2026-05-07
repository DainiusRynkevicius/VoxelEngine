//
// Created by Dainius Rynkevicius on 07/05/2026.
//

#ifndef VOXELENGINE_BLOCKLISTUI_H
#define VOXELENGINE_BLOCKLISTUI_H
#include "ImguiDrawable.h"
#include "../world/blocks/BlockRegistry.h"

namespace Ui {
    class BlockListWindow : public ImguiDrawable {
    public:
        BlockListWindow(World::Blocks::BlockRegistry& registry);

        void Draw(double deltaTime) override;

        ~BlockListWindow() override = default;

    private:
        World::Blocks::BlockRegistry& registry;
        bool open = true;
    };
} // Ui

#endif //VOXELENGINE_BLOCKLISTUI_H
