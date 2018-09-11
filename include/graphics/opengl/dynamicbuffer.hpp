#ifndef graphics_opengl_dynamicbuffer_hpp
#define graphics_opengl_dynamicbuffer_hpp

#include "graphics/descriptors.hpp"
#include "graphics/opengl/gl.hpp"

namespace graphics::opengl {
    class Context;

    class DynamicBuffer {
        uint32_t        _size;
        uint32_t        _capacity;
        GLuint          _vbos[BUFFER_COUNT];
        uint8_t *       _buffer;

        friend class Context;

    public:
        DynamicBuffer(uint32_t size, uint32_t capacity);
        ~DynamicBuffer();

        constexpr uint32_t getSize()     const { return _size;     }
        constexpr uint32_t getCapacity() const { return _capacity; }

        constexpr uint32_t getOffset(uint32_t frame) const {
            return 0;
        }

        void update(uint32_t frame);
        void update(uint32_t frame, uint32_t offset, uint32_t length);

        void * getData(uint32_t frame) {
            return &_buffer[frame * _size * _capacity];
        }
    };
}

#endif /* graphics_opengl_dynamicbuffer_hpp */
