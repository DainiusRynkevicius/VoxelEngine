//
// Created by Dainius Rynkevicius on 24/04/2026.
//

#include "Renderer.h"

#include <ranges>

#include "FrameUniform.h"
#include "spdlog/spdlog.h"

namespace Render {
    Renderer::Renderer(Gpu::GpuContext& ctx) : pipeline(ctx) {
        wgpu::BufferDescriptor uniform_desc{};
        uniform_desc.size = sizeof(FrameUniform);
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

        CreateDepthTexture(ctx);
    }

    static bool flag = false;

    void Renderer::Render(Gpu::GpuContext& ctx, Ui::DebugUi& imgui, Camera& camera,
                          World::Blocks::BlockRegistry& registry, World::Level& level) {
        auto frame_opt = ctx.StartFrame();
        if (!frame_opt) {
            spdlog::warn("Received invalid frame, skipping frame...");
            return;
        }
        auto& frame = *frame_opt;

        float aspect = (float) frame.Width() / (float) frame.Height();
        glm::mat4 viewProj = camera.Projection(aspect) * camera.View();

        FrameUniform uniform = {viewProj};

        ctx.Queue().writeBuffer(*frame_uniform, 0, &uniform, sizeof(FrameUniform));

        //TODO: REMOVE VERY UGLY
        if (!flag) {
            for (auto& [pos, chunk]: level.GetChunks()) {
                world_mesher.GenerateMesh(pos, level, registry, ctx.Device(), ctx.Queue(), pipeline.ChunkUniform());
            }
            flag = true;
        }
        wgpu::raii::CommandEncoder encoder = ctx.Device().createCommandEncoder();

        wgpu::RenderPassDescriptor render_desc{};

        wgpu::RenderPassColorAttachment color_attachment{};
        color_attachment.clearValue = wgpu::Color(0.3, 0.1, 0.7, 1.0);
        color_attachment.loadOp = wgpu::LoadOp::Clear;
        color_attachment.storeOp = wgpu::StoreOp::Store;
        color_attachment.view = frame.ColorView();

        render_desc.colorAttachmentCount = 1;
        render_desc.colorAttachments = &color_attachment;

        wgpu::RenderPassDepthStencilAttachment depth{};
        depth.view = *depth_view;
        depth.depthLoadOp = wgpu::LoadOp::Clear;
        depth.depthClearValue = 1.0;
        depth.depthStoreOp = wgpu::StoreOp::Store;

        render_desc.depthStencilAttachment = &depth;

        {
            wgpu::raii::RenderPassEncoder pass = encoder->beginRenderPass(render_desc);
            pass->setPipeline(pipeline.Get());
            pass->setBindGroup(0, *frame_uniform_group, 0, nullptr);

            for (auto [pos, buffers]: world_mesher.GetChunkBuffers()) {
                if (buffers.index_count == 0) {
                    continue;
                }
                pass->setVertexBuffer(0, *buffers.vertex, 0, buffers.vertex->getSize());
                pass->setIndexBuffer(*buffers.index, wgpu::IndexFormat::Uint32, 0, buffers.index->getSize());
                pass->setBindGroup(1, *buffers.chunk_uniform_group, 0, nullptr);
                pass->drawIndexed(buffers.index_count, 1, 0, 0, 0);
            }

            imgui.Render(*pass);
            pass->end();
        }

        ctx.Queue().submit(encoder->finish());

        ctx.EndFrame();
    }

    void Renderer::CreateDepthTexture(Gpu::GpuContext& ctx) {
        auto surface_size = ctx.SurfaceSize();
        wgpu::Extent3D size{};
        size.depthOrArrayLayers = 1;

        size.height = surface_size.y;
        wgpu::TextureDescriptor desc{};
        desc.dimension = wgpu::TextureDimension::_2D;
        desc.format = wgpu::TextureFormat::Depth32Float;
        desc.mipLevelCount = 1;
        desc.size.width = surface_size.x;
        desc.size.height = surface_size.y;
        desc.size.depthOrArrayLayers = 1;
        desc.sampleCount = 1;
        desc.usage = wgpu::TextureUsage::RenderAttachment;

        depth_texture = ctx.Device().createTexture(desc);

        depth_view = depth_texture->createView();
    }
} // Render
