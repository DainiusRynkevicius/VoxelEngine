//
// Created by Dainius Rynkevicius on 25/04/2026.
//

#include "Pipeline.h"

#include <fstream>

#include "../Renderer.h"
#include "spdlog/spdlog.h"


namespace Render::Gpu {
    Pipeline::Pipeline(GpuContext& ctx) {
        auto shader = LoadShaderModule("assets/shaders/chunk.wgsl", ctx.Device());

        wgpu::BindGroupLayoutDescriptor uniform_desc{};
        uniform_desc.entryCount = 1;


        wgpu::BindGroupLayoutEntry uniform_entry{};
        uniform_entry.binding = 0;
        uniform_entry.visibility = wgpu::ShaderStage::Vertex;
        uniform_entry.buffer.type = wgpu::BufferBindingType::Uniform;

        uniform_desc.entries = &uniform_entry;

        frame_uniform_layout = ctx.Device().createBindGroupLayout(uniform_desc);

        wgpu::PipelineLayoutDescriptor layout_descriptor{};

        std::array<WGPUBindGroupLayout, 1> layouts = {
            *frame_uniform_layout
        };

        layout_descriptor.bindGroupLayoutCount = layouts.size();
        layout_descriptor.bindGroupLayouts = layouts.data();

        layout = ctx.Device().createPipelineLayout(layout_descriptor);

        wgpu::RenderPipelineDescriptor pipeline_desc;
        pipeline_desc.layout = *layout;
        pipeline_desc.vertex.bufferCount = 0;
        pipeline_desc.vertex.buffers = nullptr;

        pipeline_desc.vertex.module = *shader;
        pipeline_desc.vertex.entryPoint = wgpu::StringView{"vs_main"};
        pipeline_desc.vertex.constantCount = 0;
        pipeline_desc.vertex.constants = nullptr;

        const wgpu::VertexBufferLayout vertex_layout = Render::Renderer::Vertex::GetLayout();
        pipeline_desc.vertex.bufferCount = 1;
        pipeline_desc.vertex.buffers = &vertex_layout;

        pipeline_desc.primitive.cullMode = wgpu::CullMode::Back;
        pipeline_desc.primitive.frontFace = wgpu::FrontFace::CCW;
        pipeline_desc.primitive.stripIndexFormat = wgpu::IndexFormat::Undefined;
        pipeline_desc.primitive.topology = wgpu::PrimitiveTopology::TriangleList;

        wgpu::FragmentState frag_state{};
        frag_state.module = *shader;
        frag_state.entryPoint = wgpu::StringView{"fs_main"};
        frag_state.constantCount = 0;
        frag_state.constants = nullptr;

        // Setup blending
        wgpu::BlendState blend_state;
        blend_state.color.srcFactor = wgpu::BlendFactor::SrcAlpha;
        blend_state.color.dstFactor = wgpu::BlendFactor::OneMinusSrcAlpha;
        blend_state.color.operation = wgpu::BlendOperation::Add;
        blend_state.alpha.srcFactor = wgpu::BlendFactor::Zero;
        blend_state.alpha.dstFactor = wgpu::BlendFactor::One;
        blend_state.alpha.operation = wgpu::BlendOperation::Add;

        wgpu::ColorTargetState color_target;
        color_target.format = ctx.ColorFormat();
        color_target.blend = &blend_state;
        color_target.writeMask = wgpu::ColorWriteMask::All;

        frag_state.targetCount = 1;
        frag_state.targets = &color_target;

        pipeline_desc.fragment = &frag_state;

        wgpu::DepthStencilState depth{};
        depth.format = wgpu::TextureFormat::Depth32Float;
        depth.depthWriteEnabled = wgpu::OptionalBool::True;
        depth.depthCompare = wgpu::CompareFunction::Less;

        pipeline_desc.depthStencil = &depth;

        pipeline_desc.multisample.count = 1;
        pipeline_desc.multisample.mask = ~0u;
        pipeline_desc.multisample.alphaToCoverageEnabled = false;

        pipeline = ctx.Device().createRenderPipeline(pipeline_desc);
    }

    wgpu::raii::ShaderModule Pipeline::LoadShaderModule(std::filesystem::path path, wgpu::Device device) {
        spdlog::info("Loading shader: {}", path.string());
        std::ifstream in(path);
        if (!in) {
            throw std::runtime_error(std::format("Failed to open path {}", path.string()));
        }

        std::string src((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());

        wgpu::ShaderSourceWGSL wgsl{};
        wgsl.code = wgpu::StringView{src};
        wgsl.chain.sType = wgpu::SType::ShaderSourceWGSL;

        wgpu::ShaderModuleDescriptor desc{};
        desc.label = wgpu::StringView{path.string()};
        desc.nextInChain = &wgsl.chain;

        return device.createShaderModule(desc);
    }
} // Gpu
// Render
