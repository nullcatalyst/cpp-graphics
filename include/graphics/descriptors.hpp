#ifndef graphics_descriptors_hpp
#define graphics_descriptors_hpp

#include <cstdint> // uint32_t

namespace graphics {
    enum class CompareFunction : uint32_t {
        Never                   = 0,
        Less                    = 1,
        Equal                   = 2,
        LessEqual               = 3,
        Greater                 = 4,
        NotEqual                = 5,
        GreaterEqual            = 6,
        Always                  = 7,
    };

    enum class CullMode : uint32_t {
        None                    = 0,
        Front                   = 1,
        Back                    = 2,
    };

    enum class Winding : uint32_t {
        Clockwise               = 0,
        CounterClockwise        = 1,
    };

    enum class VertexFormat : uint32_t {
        Invalid                 = 0,
        Float                   = 1,
        Float2                  = 2,
        Float3                  = 3,
        Float4                  = 4,
    };

    enum class StepFunction : uint32_t {
        Constant                = 0,
        PerVertex               = 1,
        PerInstance             = 2,
    };

    enum class PixelFormat : uint32_t {
        Invalid                 = 0,

        RGB24u                  = 1,
        RGB24s                  = 2,

        RGBA32u                 = 3,
        RGBA32s                 = 4,
        BGRA32u                 = 5,

        Depth16f                = 6,
        Depth32f                = 7,
//        Depth24f_Stencil8u      = 8,
    };

    enum class StorageHint : uint32_t {
        None                    = 0,
        MipMap                  = 1,
        Memoryless              = 2,
    };

    struct VertexAttribute {
        uint32_t                id;
        VertexFormat            format;
        uint32_t                offset;
        uint32_t                bufferIndex;
    };

    struct BufferLayout {
        uint32_t                bufferIndex;
        uint32_t                stride;
        uint32_t                stepRate;
        StepFunction            stepFunction;
    };

    struct VertexDesc {
        uint32_t                attributeCount;
        uint32_t                layoutCount;
        const VertexAttribute * attributes;
        const BufferLayout *    layouts;
    };

    struct Attachment {
        uint32_t                id;
        void *                  texture;
        bool                    clear;
    };

    struct RenderTargetDesc {
        uint32_t                sampleCount;
        uint32_t                colorAttachmentCount;
        const Attachment *      colorAttachments;
        Attachment              depthAttachment;
    };

    // This is used to create an initialized (opaque) State object
    struct StateDesc {
        const char *                vertexShader;
        const char *                fragmentShader;
        const VertexDesc *          vertexDesc;
        const RenderTargetDesc *    targetDesc;
        CullMode                    cullMode;
        Winding                     winding;
        bool                        depthWriteEnabled;
        CompareFunction             depthCompareFunction;
        float                       depthBias;
        float                       depthSlopeScale;
        float                       depthBiasClamp;
    };

    struct TextureDesc {
        uint32_t    width;
        uint32_t    height;
        PixelFormat format;
        StorageHint storage;
    };
}

#endif /* graphics_descriptors_hpp */
