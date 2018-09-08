#ifndef graphics_metal_dynamicbuffer_hpp
#define graphics_metal_dynamicbuffer_hpp

#ifdef __OBJC__
    #include <Metal/Metal.h>
#endif

#include "util/os.hpp"

namespace graphics::metal {
    class Context;

    class DynamicBuffer {
        uint32_t        _size;
        uint32_t        _count;
        uint32_t        _capacity;

#ifdef __OBJC__
        id<MTLBuffer>   _buffer;
#else 
        void *          _buffer;
#endif

        friend class Context;

        static constexpr uint32_t getBufferSize(uint32_t size, uint32_t capacity) {
            return (((size * capacity) & ~0xff) + 0x100);
        }

    public:
#ifdef __OBJC__
        DynamicBuffer(uint32_t size, uint32_t capacity, id<MTLDevice> device);
#endif
        ~DynamicBuffer();

        constexpr uint32_t getSize()     const { return _size;     }
        constexpr uint32_t getCount()    const { return _count;    }
        constexpr uint32_t getCapacity() const { return _capacity; }

        constexpr uint32_t getOffset(uint32_t frame) const {
            return frame * getBufferSize(_size, _capacity);
        }

        void update(uint32_t frame);
        void update(uint32_t frame, uint32_t startByte, uint32_t endByte);

        void * getData(uint32_t frame);
    };
}

#endif /* graphics_metal_dynamicbuffer_hpp */
