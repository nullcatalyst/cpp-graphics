#ifndef graphics_opengl_convert_hpp
#define graphics_opengl_convert_hpp

#include "util/os.hpp"
#include "util/debug.hpp" // UNREACHABLE
#include "graphics/descriptors.hpp"
#include "graphics/opengl/gl.hpp"

namespace graphics::opengl {
    /// TODO: Change any switch statements to array lookups

    constexpr GLenum convert(PixelFormat format) {
        switch (format) {
            case PixelFormat::RGBA32u:                  return GL_RGBA8;
            case PixelFormat::RGBA32s:                  return GL_RGBA8_SNORM;
            case PixelFormat::BGRA32u:                  return GL_BGRA;
            case PixelFormat::Depth16f:                 return GL_R16F;
            case PixelFormat::Depth32f:                 return GL_R32F;
            default:                                    UNREACHABLE();
        }
    }

    constexpr GLenum convert(CompareFunction compareFunction) {
        switch (compareFunction) {
            case CompareFunction::Never:                return GL_NEVER;
            case CompareFunction::Less:                 return GL_LESS;
            case CompareFunction::Equal:                return GL_EQUAL;
            case CompareFunction::LessEqual:            return GL_LEQUAL;
            case CompareFunction::Greater:              return GL_GREATER;
            case CompareFunction::NotEqual:             return GL_NOTEQUAL;
            case CompareFunction::GreaterEqual:         return GL_GEQUAL;
            case CompareFunction::Always:               return GL_ALWAYS;
            default:                                    UNREACHABLE();
        }
    }

    constexpr GLenum convert(CullMode cullMode) {
        switch (cullMode) {
            case CullMode::None:                        return GL_NONE;
            case CullMode::Front:                       return GL_FRONT;
            case CullMode::Back:                        return GL_BACK;
            default:                                    UNREACHABLE();
        }
    }

    constexpr GLenum convert(Winding winding) {
        switch (winding) {
            case Winding::Clockwise:                    return GL_CW;
            case Winding::CounterClockwise:             return GL_CCW;
            default:                                    UNREACHABLE();
        }
    }

    constexpr GLenum convert(TextureFilter filter) {
        switch (filter) {
            case TextureFilter::Nearest:                return GL_NEAREST;
            case TextureFilter::Linear:                 return GL_LINEAR;
            case TextureFilter::NearestMipmapNearest:   return GL_NEAREST_MIPMAP_NEAREST;
            case TextureFilter::NearestMipmapLinear:    return GL_NEAREST_MIPMAP_LINEAR;
            case TextureFilter::LinearMipmapNearest:    return GL_LINEAR_MIPMAP_NEAREST;
            case TextureFilter::LinearMipmapLinear:     return GL_LINEAR_MIPMAP_LINEAR;
            default:                                    UNREACHABLE();
        }
    }

    constexpr GLenum convert(TextureWrap wrap) {
        switch (wrap) {
            case TextureWrap::Clamp:                    return GL_CLAMP_TO_EDGE;
            case TextureWrap::Repeat:                   return GL_REPEAT;
            case TextureWrap::RepeatMirrored:           return GL_MIRRORED_REPEAT;
            default:                                    UNREACHABLE();
        }
    }
}

#endif /* graphics_opengl_convert_hpp */
