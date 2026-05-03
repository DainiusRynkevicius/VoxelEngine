//
// Created by dainiusr on 5/3/26.
//

#ifndef VOXELENGINE_MESHDATA_H
#define VOXELENGINE_MESHDATA_H
#include <vector>

#include "ChunkVertex.h"

namespace Render {
    struct MeshData {
        std::vector<ChunkVertex> vertices;
        std::vector<uint32_t> indices;
    };
}

#endif //VOXELENGINE_MESHDATA_H
