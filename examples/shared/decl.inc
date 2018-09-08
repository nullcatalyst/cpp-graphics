struct Vertex {
    simd::float4 position;
    simd::float4 color;
};

struct Uniforms {
    simd::float4x4 matrix;
};

const Vertex VERTICES[] = {
    Vertex{ simd::float4{-0.5f, -0.5f, 0.0f, 1.0f}, simd::float4{1.0f, 0.0f, 0.0f, 1.0f} },
    Vertex{ simd::float4{+0.5f, -0.5f, 0.0f, 1.0f}, simd::float4{1.0f, 1.0f, 0.0f, 1.0f} },
    Vertex{ simd::float4{-0.5f, +0.5f, 0.0f, 1.0f}, simd::float4{0.0f, 0.0f, 1.0f, 1.0f} },
    Vertex{ simd::float4{+0.5f, +0.5f, 0.0f, 1.0f}, simd::float4{0.0f, 1.0f, 0.0f, 1.0f} },
};

const graphics::VertexAttribute VERTEX_ATTRIBS[] = {
    graphics::VertexAttribute{
        .id             = 0,
        .format         = graphics::VertexFormat::Float4,
        .offset         = offsetof(Vertex, position),
        .bufferIndex    = 0,
    },
    graphics::VertexAttribute{
        .id             = 1,
        .format         = graphics::VertexFormat::Float4,
        .offset         = offsetof(Vertex, color),
        .bufferIndex    = 0,
    },
};

const graphics::BufferLayout VERTEX_LAYOUTS[] = {
    graphics::BufferLayout{
        .bufferIndex    = 0,
        .stride         = sizeof(Vertex),
        .stepFunction   = graphics::StepFunction::PerVertex,
    },
};

const graphics::VertexDesc VERTEX_DESC{
    .attributeCount = SIZEOF_ARRAY(VERTEX_ATTRIBS),
    .layoutCount    = SIZEOF_ARRAY(VERTEX_LAYOUTS),
    .attributes     = VERTEX_ATTRIBS,
    .layouts        = VERTEX_LAYOUTS,
};

const graphics::StateDesc stateDesc{
    .vertexShader           = "default_vertex",
    .fragmentShader         = "default_fragment",
    .targetDesc             = nullptr, //< Use screen
    .vertexDesc             = &VERTEX_DESC,
    .cullMode               = graphics::CullMode::Back,
    .winding                = graphics::Winding::CounterClockwise,
    .depthCompareFunction   = graphics::CompareFunction::Always,
    .depthWriteEnabled      = true,
};
