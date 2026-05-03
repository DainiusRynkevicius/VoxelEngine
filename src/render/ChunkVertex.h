//
// Created by dainiusr on 5/3/26.
//

#ifndef VOXELENGINE_CHUNKVERTEX_H
#define VOXELENGINE_CHUNKVERTEX_H
#include "glm/glm.hpp"
#include "webgpu/webgpu.hpp"
#include <array>
#include <cstddef>

namespace Render {
    struct ChunkVertex {
        glm::vec3 position;
        glm::vec2 uv;
        uint32_t texture_layer;

        static wgpu::VertexBufferLayout GetLayout() {
            wgpu::VertexBufferLayout layout{};
            layout.arrayStride = sizeof(ChunkVertex);
            layout.stepMode = wgpu::VertexStepMode::Vertex;
            static std::array<WGPUVertexAttribute, 3> attributes;
            attributes[0].format = wgpu::VertexFormat::Float32x3;
            attributes[0].offset = offsetof(ChunkVertex, position);
            attributes[0].shaderLocation = 0;

            attributes[1].format = wgpu::VertexFormat::Float32x2;
            attributes[1].offset = offsetof(ChunkVertex, uv);
            attributes[1].shaderLocation = 1;

            attributes[2].format = wgpu::VertexFormat::Uint32;
            attributes[2].offset = offsetof(ChunkVertex, texture_layer);
            attributes[2].shaderLocation = 2;

            layout.attributeCount = attributes.size();
            layout.attributes = attributes.data();
            return layout;
        }
    };
}

#endif //VOXELENGINE_CHUNKVERTEX_H
