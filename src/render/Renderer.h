//
// Created by Dainius Rynkevicius on 24/04/2026.
//

#ifndef VOXELENGINE_RENDERER_H
#define VOXELENGINE_RENDERER_H
#include "gpu/GpuContext.h"

namespace render {

class Renderer {
public:
    Renderer(Render::Gpu::GpuContext& ctx);

    void Render(Render::Gpu::GpuContext& ctx);
private:


};

} // render

#endif //VOXELENGINE_RENDERER_H
