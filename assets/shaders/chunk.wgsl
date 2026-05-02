struct VertexIn{
    @location(0) position: vec3f,
    @location(1) uv: vec2f,
    @interpolate(flat) @location(2) layer: u32,
}

struct VertexOut{
    @builtin(position) position: vec4f,
    @location(0) layer: u32,
}

struct FrameUniform{
    viewProj: mat4x4f
}

struct ChunkUniform{
    model: mat4x4f
}

@group(0) @binding(0)
var<uniform> frame_uniform: FrameUniform;

@group(1) @binding(0)
var<uniform> chunk_uniform: ChunkUniform;

@vertex
fn vs_main(in: VertexIn) -> VertexOut{
    var out: VertexOut;
    out.position = frame_uniform.viewProj * chunk_uniform.model * vec4f(in.position,1.0);
    out.layer = in.layer;
    return out;
}


@fragment
fn fs_main(in: VertexOut) -> @location(0) vec4f{
    return vec4f(1, 1, 0, 1.0);
}