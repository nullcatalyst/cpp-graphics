#ifndef graphics_metal_staticbuffer_hpp
#define graphics_metal_staticbuffer_hpp

#include <cstdint> // uint32_t

#ifdef __OBJC__
    #include <Metal/Metal.h>
#endif

namespace graphics::metal {
    class Context;

    class StaticBuffer {
        uint32_t        _size;
        uint32_t        _capacity;

#ifdef __OBJC__
        id<MTLBuffer>   _buffer;
#else
        void *          _buffer;
#endif

        friend class Context;

    public:
#ifdef __OBJC__
        StaticBuffer(uint32_t size, uint32_t capacity, const void * data, id<MTLDevice> device);
#endif
        ~StaticBuffer();

        constexpr uint32_t getSize()     const { return _size;     }
        constexpr uint32_t getCapacity() const { return _capacity; }
    };
}

#endif /* graphics_metal_staticbuffer_hpp */
