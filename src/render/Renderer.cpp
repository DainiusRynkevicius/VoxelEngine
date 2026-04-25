//
// Created by Dainius Rynkevicius on 24/04/2026.
//

#include "Renderer.h"
#include "Renderer.h"

#include "spdlog/spdlog.h"

namespace render {
    Renderer::Renderer(Render::Gpu::GpuContext& ctx) : pipeline(ctx) {
        //TODO: move elsewhere
        wgpu::BufferDescriptor buffer_desc{};
        buffer_desc.size = sizeof(Vertex) * vertices.size();
        buffer_desc.mappedAtCreation = false;
        buffer_desc.usage = wgpu::BufferUsage::Vertex | wgpu::BufferUsage::CopyDst;

        vertex_buffer = ctx.Device().createBuffer(buffer_desc);

        ctx.Queue().writeBuffer(*vertex_buffer, 0, vertices.data(), buffer_desc.size);
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
            pass->setPipeline(pipeline.Get());
            pass->setVertexBuffer(0, *vertex_buffer, 0, vertex_buffer->getSize());
            pass->draw(vertices.size(), 1, 0, 0);
            pass->end();
        }

        ctx.Queue().submit(encoder->finish());

        ctx.EndFrame();
    }
} // render