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
        src/Window.cpp
        src/Window.h
        src/render/Renderer.cpp
        src/render/Renderer.h
        src/render/gpu/Frame.h
        src/render/gpu/Pipeline.cpp
        src/render/gpu/Pipeline.h
        src/ui/ImguiDrawable.h
        src/ui/DemoWindow.cpp
        src/ui/DemoWindow.h
        src/ui/DebugUi.cpp
        src/ui/DebugUi.h
        src/ui/PerformanceWindow.cpp
        src/ui/PerformanceWindow.h
        src/render/Camera.cpp
        src/render/Camera.h
        src/render/FrameUniform.h
        src/world/Level.cpp
        src/world/Level.h
        src/world/Chunk.cpp
        src/world/Chunk.h
        src/world/blocks/Block.h
        src/world/blocks/BlockRegistry.cpp
        src/world/blocks/BlockRegistry.h
        src/world/generators/Generator.h
        src/world/generators/FlatGenerator.cpp
        src/world/generators/FlatGenerator.h
        src/render/ChunkMesher.cpp
        src/render/ChunkMesher.h
        src/render/ChunkUniform.h
        src/CameraController.cpp
        src/CameraController.h
        src/render/ChunkVertex.h
        src/render/ChunkMesh.h
        src/render/MeshData.h
        src/render/ChunkMesh.cpp
)

target_link_libraries(VoxelEngine webgpu spdlog glfw glfw3webgpu glm imgui)

add_custom_target(CopyAssets ALL COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/assets $<TARGET_FILE_DIR:VoxelEngine>/assets COMMENT "Copying Assets")

add_dependencies(VoxelEngine CopyAssets)