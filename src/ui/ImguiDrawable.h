//
// Created by Dainius Rynkevicius on 25/04/2026.
//

#ifndef VOXELENGINE_IMGUIDRAWABLE_H
#define VOXELENGINE_IMGUIDRAWABLE_H

namespace Ui {
    class ImguiDrawable {
    public:
        virtual ~ImguiDrawable() = default;

        virtual void Draw(double deltaTime) = 0;
    };
}

#endif //VOXELENGINE_IMGUIDRAWABLE_H
