#ifndef graphics_opengl_rendertarget_hpp
#define graphics_opengl_rendertarget_hpp

#include "graphics/opengl/gl.hpp"

namespace graphics::opengl {
    class Context;

    class RenderTarget {
        GLuint  _fbo;

        friend class Context;

    public:
        // RenderTarget();
        ~RenderTarget();
    };
}

#endif /* graphics_opengl_rendertarget_hpp */
