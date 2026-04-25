//
// Created by Dainius Rynkevicius on 24/04/2026.
//

#include "Renderer.h"

#include "FrameUniform.h"
#include "spdlog/spdlog.h"

namespace render {
    Renderer::Renderer(Render::Gpu::GpuContext& ctx) : pipeline(ctx) {
        //TODO: move elsewhere
        wgpu::BufferDescriptor vert_buffer_desc{};
        vert_buffer_desc.size = sizeof(Vertex) * vertices.size();
        vert_buffer_desc.mappedAtCreation = false;
        vert_buffer_desc.usage = wgpu::BufferUsage::Vertex | wgpu::BufferUsage::CopyDst;

        vertex_buffer = ctx.Device().createBuffer(vert_buffer_desc);
        ctx.Queue().writeBuffer(*vertex_buffer, 0, vertices.data(), vert_buffer_desc.size);

        wgpu::BufferDescriptor uniform_desc{};
        uniform_desc.size = sizeof(Render::FrameUniform);
        uniform_desc.mappedAtCreation = false;
        uniform_desc.usage = wgpu::BufferUsage::CopyDst | wgpu::BufferUsage::Uniform;

        frame_uniform = ctx.Device().createBuffer(uniform_desc);

        wgpu::BindGroupDescriptor uniform_group_desc{};
        uniform_group_desc.layout = pipeline.FrameUniform();
        wgpu::BindGroupEntry entry{};
        entry.size = uniform_desc.size;
        entry.binding = 0;
        entry.buffer = *frame_uniform;
        entry.offset = 0;
        uniform_group_desc.entryCount = 1;
        uniform_group_desc.entries = &entry;

        frame_uniform_group = ctx.Device().createBindGroup(uniform_group_desc);
    }

    void Renderer::Render(Render::Gpu::GpuContext& ctx, Ui::DebugUi& imgui, Render::Camera& camera) {
        auto frame_opt = ctx.StartFrame();
        if (!frame_opt) {
            spdlog::warn("Received invalid frame, skipping frame...");
            return;
        }
        auto& frame = *frame_opt;

        float aspect = (float) frame.Width() / (float) frame.Height();
        glm::mat4 viewProj = camera.Projection(aspect) * camera.View();
        glm::mat4 model = glm::translate(glm::mat4{1.0f}, glm::vec3(0.0f, 0.0f, -3.0f));
        //glm::mat4 model = glm::mat4{1.0};

        Render::FrameUniform uniform = {viewProj, model};

        ctx.Queue().writeBuffer(*frame_uniform, 0, &uniform, sizeof(Render::FrameUniform));

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
            pass->setBindGroup(0, *frame_uniform_group, 0, nullptr);
            pass->draw(vertices.size(), 1, 0, 0);

            imgui.Render(*pass);
            pass->end();
        }

        ctx.Queue().submit(encoder->finish());

        ctx.EndFrame();
    }
} // render