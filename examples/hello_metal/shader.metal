#include <metal_stdlib>
using namespace metal;

struct Vertex {
    float4      position    [[ attribute(0) ]];
    float4      color       [[ attribute(1) ]];
};

struct Uniforms {
    float4x4    matrix;
};

struct InOut {
    float4 position [[ position ]];
    half4 color;
};

vertex InOut default_vertex(Vertex in [[ stage_in ]], constant Uniforms & uniforms [[ buffer(1) ]]) {
    InOut out;
    out.position = uniforms.matrix * in.position;
    out.color = half4(in.color);
    return out;
}

fragment half4 default_fragment(InOut in [[ stage_in ]]) {
    return in.color;
}
