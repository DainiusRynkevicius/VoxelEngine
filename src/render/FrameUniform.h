//
// Created by Dainius Rynkevicius on 25/04/2026.
//

#ifndef VOXELENGINE_FRAMEUNIFORM_H
#define VOXELENGINE_FRAMEUNIFORM_H

#include <glm/glm.hpp>

namespace Render{
    struct FrameUniform {
        glm::mat4 viewProj;
    };
}

#endif //VOXELENGINE_FRAMEUNIFORM_H
