#include "graphics/metal/dynamicbuffer.hpp"

#include "util/debug.hpp" // ASSERT
#include "graphics/metal/context.hpp" // BUFFER_COUNT

namespace graphics::metal {
    DynamicBuffer::DynamicBuffer(uint32_t size, uint32_t capacity, id<MTLDevice> device) {
        _size       = size;
        _count      = 0;
        _capacity   = capacity;

        const uint32_t totalSize = BUFFER_COUNT * getBufferSize(size, capacity);

#if OS(mac)
        // macOS performs better with explicit copying of only the modified buffer range from CPU to GPU
        _buffer = [device newBufferWithLength:totalSize options:MTLResourceStorageModeManaged];
#elif OS(ios)
        // iOS has only a single unified memory address space, so sharing the buffer between CPU and GPU is easy
        _buffer = [device newBufferWithLength:totalSize options:MTLResourceStorageModeShared];
#else
    #error "Unknown target OS"
#endif
    }

    DynamicBuffer::~DynamicBuffer() {
        _buffer = nil;
    }

    void DynamicBuffer::update(uint32_t frame) {
#if OS(mac)
        const NSRange range{
            .location   = getOffset(frame),
            .length     = getBufferSize(_size, _capacity),
        };

        [_buffer didModifyRange:range];
#endif
    }

    void DynamicBuffer::update(uint32_t frame, uint32_t startByte, uint32_t endByte) {
#if OS(mac)
        const NSRange range{
            .location   = getOffset(frame) + startByte,
            .length     = endByte - startByte,
        };

        [_buffer didModifyRange:range];
#endif
    }

    void * DynamicBuffer::getData(uint32_t frame) {
        ASSERT(frame < BUFFER_COUNT, "");

        return ((uint8_t *) _buffer.contents) + frame * getBufferSize(_size, _capacity);
    }
}
