cmake_minimum_required(VERSION 3.28)
project(VoxelEngine)

set(CMAKE_CXX_STANDARD 23)

add_subdirectory(libs)

add_executable(VoxelEngine src/main.cpp)

target_link_libraries(VoxelEngine webgpu spdlog glfw glfw3webgpu glm)