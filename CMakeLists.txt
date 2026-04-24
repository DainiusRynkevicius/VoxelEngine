cmake_minimum_required(VERSION 3.28)
project(VoxelEngine)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_C_STANDARD 11)

add_subdirectory(libs)

add_executable(VoxelEngine src/main.cpp
        src/render/gpu/GpuContext.cpp
        src/render/gpu/GpuContext.h
        src/Application.cpp
        src/Application.h
        src/render/Window.cpp
        src/render/Window.h
        src/render/Renderer.cpp
        src/render/Renderer.h
        src/render/gpu/Frame.h)

target_link_libraries(VoxelEngine webgpu spdlog glfw glfw3webgpu glm)