# Voxel Engine

Coursework submission for Object-Oriented Programming.\
A C++23 voxel engine build on WGPU, featuring chunk-based worlds, multiple runtime-switchable terrain generators,
textured blocks and DearImGui support.

## Demo

TODO

## Features

- Cross-platform(Windows, MacOS, Linux(Wayland tested))
- 8x8x8 chunks stored in a hashmap keyed by chunk coordinate.
- Three terrain generators:
    - Flat Generator
    - Hill Generator
    - Cave Generator
- Textured blocks, each block uses one 128x96 texture atlas
- Flycam
- ImGui debug window: performance graph, block reigstry table, world generation window.

## Controls

- W A S D or arrow keys for movement.
- Space to move up.
- Left ctrl to move down.
- Right mouse button when held enables camera looking.