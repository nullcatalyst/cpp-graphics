#include "graphics/metal/staticbuffer.hpp"

namespace graphics::metal {
    StaticBuffer::StaticBuffer(uint32_t size, uint32_t capacity, const void * data, id<MTLDevice> device) : _size(size), _capacity(capacity) {
        _buffer = [device newBufferWithBytes:data length:size * capacity options:0];
    }

    StaticBuffer::~StaticBuffer() {
        _buffer = nil;
    }

    void * StaticBuffer::getData() {
        return [_buffer contents];
    }
}
