#ifndef graphics_opengl_state_hpp
#define graphics_opengl_state_hpp

#include "graphics/opengl/gl.hpp"
#include "graphics/descriptors.hpp"

namespace graphics::opengl {
    class Context;

    class State {
        GLuint                      _shaderProgram;
        GLenum                      _cullMode;
        GLenum                      _winding;
        bool                        _depthWriteEnabled;
        GLenum                      _depthCompareFunction;
        float                       _depthBias;
        float                       _depthSlopeScale;
        // float                       _depthBiasClamp;

        GLuint                      _vao;
        VertexDesc                  _vertexDesc;

        friend class Context;

    public:
        ~State();
    };
}

#endif /* graphics_opengl_state_hpp */
