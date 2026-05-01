//
// Created by Dainius Rynkevicius on 25/04/2026.
//

#ifndef VOXELENGINE_IMGUICONTEXT_H
#define VOXELENGINE_IMGUICONTEXT_H
#include "ImguiDrawable.h"
#include "../render/gpu/GpuContext.h"

namespace Ui {
    class DebugUi {
    public:
        DebugUi(Render::Gpu::GpuContext& gpu, Window& window);

        ~DebugUi();

        void Update(double deltaTime) const;

        void Render(wgpu::RenderPassEncoder pass);

        void AddDrawable(std::unique_ptr<ImguiDrawable> drawable);

        template<std::derived_from<ImguiDrawable> T, typename... Args>
        void AddDrawable(Args&&... args) {
            drawables.push_back(std::make_unique<T>(std::forward<Args>(args)...));
        }

    private:
        std::vector<std::unique_ptr<ImguiDrawable> > drawables{};
    };
} // Ui

#endif //VOXELENGINE_IMGUICONTEXT_H
