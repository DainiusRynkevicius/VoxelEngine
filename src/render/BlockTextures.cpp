//
// Created by dainiusr on 5/3/26.
//

#include "BlockTextures.h"

#include "gpu/GpuContext.h"
#include "spdlog/spdlog.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stbimage/stb_image.h>

namespace Render {
    BlockTextures::BlockTextures(Gpu::GpuContext& ctx, World::Blocks::BlockRegistry &registry,
        wgpu::BindGroupLayout layout) {
        auto texture_paths = registry.TextureNames();

        wgpu::TextureDescriptor tex_desc{};
        tex_desc.size.width = TEXTURE_SIZE.x;
        tex_desc.size.height = TEXTURE_SIZE.y;
        tex_desc.size.depthOrArrayLayers = texture_paths.size();

        tex_desc.label = wgpu::StringView("Block Textures");
        tex_desc.dimension = wgpu::TextureDimension::_2D;
        tex_desc.format = wgpu::TextureFormat::RGBA8UnormSrgb;
        tex_desc.mipLevelCount = 1;
        tex_desc.sampleCount = 1;
        tex_desc.usage = wgpu::TextureUsage::CopyDst | wgpu::TextureUsage::TextureBinding;
        tex_desc.viewFormatCount = 0;
        tex_desc.viewFormats = nullptr;

        texture = ctx.Device().createTexture(tex_desc);

        wgpu::SamplerDescriptor sampler_desc{};
        sampler_desc.addressModeU = wgpu::AddressMode::ClampToEdge;
        sampler_desc.addressModeV = wgpu::AddressMode::ClampToEdge;
        sampler_desc.addressModeW = wgpu::AddressMode::ClampToEdge;
        sampler_desc.mipmapFilter = wgpu::MipmapFilterMode::Nearest;
        sampler_desc.magFilter = wgpu::FilterMode::Linear;
        sampler_desc.minFilter = wgpu::FilterMode::Nearest;
        sampler_desc.maxAnisotropy = 1;

        sampler = ctx.Device().createSampler(sampler_desc);

        wgpu::TextureViewDescriptor view_desc{};
        view_desc.label = wgpu::StringView("Block Texture Array View");
        view_desc.format = wgpu::TextureFormat::RGBA8UnormSrgb;
        view_desc.dimension = wgpu::TextureViewDimension::_2DArray;
        view_desc.baseMipLevel = 0;
        view_desc.mipLevelCount = 1;
        view_desc.baseArrayLayer = 0;
        view_desc.arrayLayerCount = texture_paths.size();
        view_desc.aspect = wgpu::TextureAspect::All;

        view = texture->createView(view_desc);

        wgpu::BindGroupEntry tex_entry{};
        tex_entry.binding = 0;
        tex_entry.textureView = *view;

        wgpu::BindGroupEntry samp_entry{};
        samp_entry.binding = 1;
        samp_entry.sampler = *sampler;

        std::array<wgpu::BindGroupEntry, 2> entries = {
            tex_entry,
            samp_entry
        };

        wgpu::BindGroupDescriptor bind_desc{};
        bind_desc.entries = entries.data();
        bind_desc.entryCount = entries.size();
        bind_desc.label = wgpu::StringView("Block Texture Bind Group");
        bind_desc.layout = layout;

        group = ctx.Device().createBindGroup(bind_desc);

        spdlog::info("Loading block textures...");

        wgpu::TexelCopyTextureInfo copy_info{};
        copy_info.texture = *texture;
        copy_info.aspect = wgpu::TextureAspect::All;
        copy_info.mipLevel = 0;

        wgpu::TexelCopyBufferLayout copy_layout{};
        copy_layout.bytesPerRow = 4 * TEXTURE_SIZE.x;
        copy_layout.rowsPerImage = TEXTURE_SIZE.y;

        wgpu::Extent3D copy_size = {TEXTURE_SIZE.x, TEXTURE_SIZE.y, 1};

        for (uint32_t i = 0; i < texture_paths.size(); ++i) {
            auto& path = texture_paths[i];
            spdlog::info("Loading {}...", path);
            int width, height, channels;

            unsigned char* pixels = stbi_load(path.c_str(), &width, &height, &channels, 4);

            if (!pixels) {
                spdlog::error("Texture {} failed load.", path);
                continue;
            }

            if (width != TEXTURE_SIZE.x || height != TEXTURE_SIZE.y) {
                spdlog::warn("Texture {} has invalid size. Skipping...", path);
                stbi_image_free(pixels);
                continue;
            }

            copy_info.origin = {0, 0, i};

            ctx.Queue().writeTexture(copy_info, pixels, 4*TEXTURE_SIZE.x*TEXTURE_SIZE.y,copy_layout, copy_size);

            layers.emplace(path,i);
            stbi_image_free(pixels);

            spdlog::info("Texture {} has been successfully loaded.", path);
        }
    }

    uint32_t BlockTextures::GetLayer(const std::string& texture_name) {
        return layers[texture_name];
    }
} // Render