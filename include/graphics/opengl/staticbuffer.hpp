#ifndef graphics_opengl_staticbuffer_hpp
#define graphics_opengl_staticbuffer_hpp

#include <cstdint> // uint32_t

#include "graphics/opengl/gl.hpp"

namespace graphics::opengl {
    class Context;

    class StaticBuffer {
        uint32_t        _size;
        uint32_t        _capacity;
        GLuint          _vbo;

        friend class Context;

    public:
        StaticBuffer(uint32_t size, uint32_t capacity, const void * data);
        ~StaticBuffer();

        constexpr uint32_t getSize()     const { return _size;     }
        constexpr uint32_t getCapacity() const { return _capacity; }
    };
}

#endif /* graphics_opengl_staticbuffer_hpp */
