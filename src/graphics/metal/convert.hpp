#ifndef graphics_metal_convert_hpp
#define graphics_metal_convert_hpp

#include <Metal/Metal.h>

#include "util/os.hpp"
#include "util/debug.hpp" // UNREACHABLE
#include "graphics/descriptors.hpp"

namespace graphics::metal {
    /// TODO: Change any switch statements to array lookups

    constexpr MTLPixelFormat convert(PixelFormat format) {
        switch (format) {
            case PixelFormat::RGBA32u:  return MTLPixelFormatRGBA8Unorm;
            case PixelFormat::RGBA32s:  return MTLPixelFormatRGBA8Snorm;
            case PixelFormat::BGRA32u:  return MTLPixelFormatBGRA8Unorm;
#if OS(mac)
            case PixelFormat::Depth16f: return MTLPixelFormatDepth16Unorm;
#endif
            case PixelFormat::Depth32f: return MTLPixelFormatDepth32Float;
            default:                    UNREACHABLE();
        }
    }

    constexpr MTLResourceOptions convert(StorageHint storage) {
        switch (storage) {
            case StorageHint::None:         return MTLResourceStorageModePrivate;
            case StorageHint::MipMap:       return MTLResourceStorageModePrivate;
#if OS(ios)
            case StorageHint::Memoryless:   return MTLResourceStorageModeMemoryless;
#endif
            default:                        UNREACHABLE();
        }
    }

    constexpr MTLCompareFunction convert(CompareFunction compareFunction) {
        // These are currently 1-to-1
        return (MTLCompareFunction) compareFunction;
    }

    constexpr MTLCullMode convert(CullMode cullMode) {
        // These are currently 1-to-1
        return (MTLCullMode) cullMode;
    }

    constexpr MTLWinding convert(Winding winding) {
        // These are currently 1-to-1
        return (MTLWinding) winding;
    }

    constexpr MTLVertexFormat convert(VertexFormat format) {
        switch (format) {
            case VertexFormat::Float:       return MTLVertexFormatFloat;
            case VertexFormat::Float2:      return MTLVertexFormatFloat2;
            case VertexFormat::Float3:      return MTLVertexFormatFloat3;
            case VertexFormat::Float4:      return MTLVertexFormatFloat4;
            default:                        UNREACHABLE();
        }
    }

    constexpr MTLVertexStepFunction convert(StepFunction stepFunction) {
        // These are currently 1-to-1
        return (MTLVertexStepFunction) stepFunction;
    }
}

#endif /* graphics_metal_convert_hpp */
