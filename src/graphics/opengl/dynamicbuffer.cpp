#include "graphics/opengl/dynamicbuffer.hpp"

#include <cstdint> // uint8_t
#include <cstdlib> // malloc, free

namespace graphics::opengl {
    DynamicBuffer::DynamicBuffer(uint32_t size, uint32_t capacity) : _size(size), _capacity(capacity) {
        glGenBuffers(BUFFER_COUNT, _vbos);
        _buffer = (uint8_t *) malloc(size * capacity);

        for (int i = 0; i < BUFFER_COUNT; ++i) {
            glBindBuffer(GL_ARRAY_BUFFER, _vbos[i]);
            glBufferData(GL_ARRAY_BUFFER, size * capacity, nullptr, GL_DYNAMIC_DRAW);
        }
    }

    DynamicBuffer::~DynamicBuffer() {
        glDeleteBuffers(1, _vbos);
        free(_buffer);
    }

    void DynamicBuffer::update(uint32_t frame) {
        glBindBuffer(GL_ARRAY_BUFFER, _vbos[frame]);
        glBufferData(GL_ARRAY_BUFFER, _size * _capacity, getData(frame), GL_DYNAMIC_DRAW);
    }

    void DynamicBuffer::update(uint32_t frame, uint32_t offset, uint32_t length) {
        glBindBuffer(GL_ARRAY_BUFFER, _vbos[frame]);
        glBufferSubData(GL_ARRAY_BUFFER, offset, length, getData(frame));
    }
}
