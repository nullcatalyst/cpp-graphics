#include "graphics/opengl/staticbuffer.hpp"

namespace graphics::opengl {
    StaticBuffer::StaticBuffer(uint32_t size, uint32_t capacity, const void * data) : _size(size), _capacity(capacity) {
        glGenBuffers(1, &_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        glBufferData(GL_ARRAY_BUFFER, size * capacity, data, GL_STATIC_DRAW);
    }

    StaticBuffer::~StaticBuffer() {
        glDeleteBuffers(1, &_vbo);
    }
}
