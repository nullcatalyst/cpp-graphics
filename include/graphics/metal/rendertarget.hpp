#ifndef graphics_metal_rendertarget_hpp
#define graphics_metal_rendertarget_hpp

#ifdef __OBJC__
    #include <Metal/Metal.h>
#endif

namespace graphics::metal {
    class Context;

    class RenderTarget {
#ifdef __OBJC__
        MTLRenderPassDescriptor *   _renderPassDescriptor;
#else
        void *                      _renderPassDescriptor;
#endif

        friend class Context;

    public:
#ifdef __OBJC__
        RenderTarget(MTLRenderPassDescriptor * renderPassDescriptor);
#endif
        ~RenderTarget();
    };
}

#endif /* graphics_metal_rendertarget_hpp */
