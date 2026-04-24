//
// Created by Dainius Rynkevicius on 24/04/2026.
//

#include "Renderer.h"

#include "spdlog/spdlog.h"

namespace render {
    Renderer::Renderer(Render::Gpu::GpuContext& ctx) {
        //TODO: init pipelines
    }

    void Renderer::Render(Render::Gpu::GpuContext& ctx) {
        auto frame_opt = ctx.StartFrame();
        if (!frame_opt) {
            spdlog::warn("Received invalid frame, skipping frame...");
            return;
        }

        auto& frame = *frame_opt;
        wgpu::raii::CommandEncoder encoder = ctx.Device().createCommandEncoder();

        wgpu::RenderPassDescriptor render_desc{};

        wgpu::RenderPassColorAttachment color_attachment{};
        color_attachment.clearValue = wgpu::Color(0.3, 0.1, 0.7, 1.0);
        color_attachment.loadOp = wgpu::LoadOp::Clear;
        color_attachment.storeOp = wgpu::StoreOp::Store;
        color_attachment.view = frame.ColorView();

        render_desc.colorAttachmentCount = 1;
        render_desc.colorAttachments = &color_attachment;

        {
            wgpu::raii::RenderPassEncoder pass = encoder->beginRenderPass(render_desc);
            pass->end();
        }

        ctx.Queue().submit(encoder->finish());

        ctx.EndFrame();
    }
} // render