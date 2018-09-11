#ifndef graphics_opengl_texture_hpp
#define graphics_opengl_texture_hpp

#include "graphics/opengl/gl.hpp"

namespace graphics::opengl {
    class Context;
    class RenderTarget;

    class Texture {
        GLenum  _textureType;
        GLuint  _texture;

        friend class Context;
        friend class RenderTarget;

    public:
        ~Texture();
    };
}

#endif /* graphics_opengl_texture_hpp */
