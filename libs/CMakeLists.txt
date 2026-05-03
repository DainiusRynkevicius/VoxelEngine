add_subdirectory(wgpu-native)
add_subdirectory(spdlog-1.17.0)
add_subdirectory(glfw-3.4)
add_subdirectory(glfw3webgpu)
add_subdirectory(glm)
add_subdirectory(stbimage)
add_subdirectory(FastNoise2-1.1.1)

add_library(imgui imgui-docking/imgui.cpp imgui-docking/imgui_demo.cpp imgui-docking/imgui_draw.cpp imgui-docking/imgui_tables.cpp imgui-docking/imgui_widgets.cpp imgui-docking/backends/imgui_impl_glfw.cpp imgui-docking/backends/imgui_impl_wgpu.cpp)
target_include_directories(imgui PUBLIC imgui-docking/ imgui-docking/backends/)
target_link_libraries(imgui PUBLIC webgpu glfw)
target_compile_definitions(imgui PUBLIC IMGUI_IMPL_WEBGPU_BACKEND_WGPU)

# Backend uses apple rendering code
if(APPLE)
    set_source_files_properties(
            imgui-docking/backends/imgui_impl_wgpu.cpp
            PROPERTIES
            COMPILE_FLAGS "-x objective-c++"
    )
endif()

# Bug in glfw3webgpu not detecting wayland
if (UNIX AND NOT APPLE)
    if (DEFINED ENV{WAYLAND_DISPLAY} OR "$ENV{XDG_SESSION_TYPE}" STREQUAL "wayland")
        target_compile_definitions(glfw3webgpu PRIVATE _GLFW_WAYLAND)
    endif()
endif()