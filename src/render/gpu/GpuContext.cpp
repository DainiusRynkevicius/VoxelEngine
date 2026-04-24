//
// Created by Dainius Rynkevicius on 24/04/2026.
//

#define WEBGPU_CPP_IMPLEMENTATION
#include "GpuContext.h"
#include "glfw3webgpu.h"
#include "spdlog/spdlog.h"

Render::Gpu::GpuContext::GpuContext(Window& window) {
    instance = wgpu::createInstance();

    surface = wgpu::Surface(glfwGetWGPUSurface(*instance, window.Get()));

    wgpu::RequestAdapterOptions adapter_options{};
    adapter_options.compatibleSurface = *surface;
    adapter = instance->requestAdapter(adapter_options);

    wgpu::DeviceDescriptor device_desc{};
    wgpu::DeviceLostCallbackInfo lost_callback_info{};
    lost_callback_info.mode = wgpu::CallbackMode::WaitAnyOnly;
    lost_callback_info.callback = [](WGPUDevice const * device, WGPUDeviceLostReason reason, WGPUStringView message, WGPU_NULLABLE void* userdata1, WGPU_NULLABLE void* userdata2) {
        std::string reason_txt;
        switch (reason) {
            case wgpu::DeviceLostReason::Destroyed: reason_txt = "Destroyed"; break;
            case wgpu::DeviceLostReason::FailedCreation: reason_txt = "Failed creation"; break;
            case wgpu::DeviceLostReason::InstanceDropped: reason_txt = "Instance dropped"; break;
            default: reason_txt = "Unknown"; break;
        }

        std::string_view message_view{message.data, message.length};
        spdlog::error("Device lost, reason: {}, message: {}", reason_txt, message_view);
    };
    device_desc.deviceLostCallbackInfo = lost_callback_info;

    wgpu::UncapturedErrorCallbackInfo uncaptured_info{};
    uncaptured_info.callback = [](WGPUDevice const * device, WGPUErrorType type, WGPUStringView message, WGPU_NULLABLE void* userdata1, WGPU_NULLABLE void* userdata2) {
        std::string error_type;
        switch (type) {
            case wgpu::ErrorType::Internal: error_type = "Internal"; break;
            case wgpu::ErrorType::OutOfMemory: error_type = "Out of memory"; break;
            case wgpu::ErrorType::Validation: error_type = "Validation"; break;
            default: error_type = "Unknown";
        }

        std::string_view message_view{message.data, message.length};
        spdlog::error("Uncaptured error, type: {}, message: {}", error_type, message_view);
    };
    device_desc.uncapturedErrorCallbackInfo = uncaptured_info;


    device = adapter->requestDevice(device_desc);

    queue = device->getQueue();

    //TODO: Pick surface capabilities better
    wgpu::SurfaceCapabilities surface_caps;
    surface -> getCapabilities(*adapter, &surface_caps);

    auto window_size = window.Size();

    surface_configuration.device = *device;
    surface_configuration.width = window_size.x;
    surface_configuration.height = window_size.y;
    surface_configuration.alphaMode = wgpu::CompositeAlphaMode::Auto;
    surface_configuration.format = surface_caps.formats[0];
    surface_configuration.presentMode = surface_caps.presentModes[0];
    surface_configuration.usage = wgpu::TextureUsage::RenderAttachment;

    surface->configure(surface_configuration);
}

std::optional<Render::Gpu::Frame> Render::Gpu::GpuContext::StartFrame() {
    wgpu::SurfaceTexture surface_texture;
    surface->getCurrentTexture(&surface_texture);

    if (surface_texture.status != wgpu::SurfaceGetCurrentTextureStatus::SuccessOptimal &&
        surface_texture.status != wgpu::SurfaceGetCurrentTextureStatus::SuccessSuboptimal) {
        if (surface_texture.texture) {
            wgpuTextureRelease(surface_texture.texture);
        }

        //TODO: Handle better, reconfigure surface
        spdlog::warn("Failed to obtain surface texture.");
        return {};
    }

    wgpu::TextureViewDescriptor view_desc{};
    view_desc.format = wgpuTextureGetFormat(surface_texture.texture);
    view_desc.usage = wgpu::TextureUsage::RenderAttachment;
    view_desc.aspect = wgpu::TextureAspect::All;
    view_desc.arrayLayerCount = 1;
    view_desc.baseArrayLayer = 0;
    view_desc.mipLevelCount = 1;
    view_desc.baseMipLevel = 0;
    view_desc.dimension = wgpu::TextureViewDimension::_2D;

    wgpu::raii::Texture color_tex = wgpu::Texture(surface_texture.texture);
    wgpu::raii::TextureView color_view = color_tex->createView(view_desc);
    return Frame(color_view, color_tex, surface_configuration.width, surface_configuration.height);
}

void Render::Gpu::GpuContext::EndFrame() {
    wgpu::Status present_status = surface->present();
    if (present_status != wgpu::Status::Success) {
        spdlog::error("Failed to present frame");
    }
}

void Render::Gpu::GpuContext::Resize(int width, int height, GLFWwindow* window) {
    // Some platforms call resize event with (0, 0) when minimizing.
    if (width == 0 || height == 0) {
        return;
    }
    surface_configuration.width = width;
    surface_configuration.height = height;

    surface->configure(surface_configuration);
}
