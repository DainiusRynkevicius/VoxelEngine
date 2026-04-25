struct VertexIn{
    @location(0) position: vec3f,
    @location(1) color: vec3f,
}

struct VertexOut{
    @builtin(position) position: vec4f,
    @location(0) color: vec3f,
}

struct FrameUniform{
    viewProj: mat4x4f,
    model: mat4x4f
}

@group(0) @binding(0)
var<uniform> frame_uniform: FrameUniform;

@vertex
fn vs_main(in: VertexIn) -> VertexOut{
    var out: VertexOut;
    out.color = in.color;
    out.position = frame_uniform.viewProj * frame_uniform.model * vec4f(in.position,1.0);
    return out;
}


@fragment
fn fs_main(in: VertexOut) -> @location(0) vec4f{
    return vec4f(in.color, 1.0);
}