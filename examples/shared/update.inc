static float angle = 0.0f;
angle += 0.5f * M_PI / 60.0f;

const uint32_t frame = _context->getFrame();

float c = cosf(angle);
float s = sinf(angle);

Uniforms * uniforms = (Uniforms *) _uniformsBuffer->getData(frame);
uniforms->matrix = simd::float4x4{
    simd::float4{    c,    s, 0.0f, 0.0f },
    simd::float4{   -s,    c, 0.0f, 0.0f },
    simd::float4{ 0.0f, 0.0f, 1.0f, 0.0f },
    simd::float4{ 0.0f, 0.0f, 0.0f, 1.0f },
};

_uniformsBuffer->update(frame);
