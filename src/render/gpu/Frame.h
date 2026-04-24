//
// Created by Dainius Rynkevicius on 24/04/2026.
//

#ifndef VOXELENGINE_FRAME_H
#define VOXELENGINE_FRAME_H
#include <utility>

#include "webgpu/webgpu-raii.hpp"

namespace Render::Gpu {
    class Frame {
    public:
        // No copy
        Frame(const Frame&) = delete;
        Frame& operator=(const Frame&) = delete;

        // allow move
        Frame(Frame&&)  noexcept = default;
        Frame& operator=(Frame&&)  noexcept = default;

        Frame(wgpu::raii::TextureView color_view, wgpu::raii::Texture color_texture, const int width,
              const int height) : color_view(color_view), color_texture(color_texture), width(width),
                            height(height) {
        }

        wgpu::TextureView ColorView() {
            return *color_view;
        }

        [[nodiscard]] int Width() const {
            return width;
        }

        [[nodiscard]] int Height() const {
            return height;
        }

    private:
        wgpu::raii::TextureView color_view;
        wgpu::raii::Texture color_texture;;
        int width;
        int height;
    };
}

#endif //VOXELENGINE_FRAME_H
