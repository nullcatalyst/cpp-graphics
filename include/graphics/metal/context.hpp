#ifndef graphics_metal_context_hpp
#define graphics_metal_context_hpp

#include <cstdint> // uint32_t

#ifdef __OBJC__
    #include <Metal/Metal.h>
    #include <MetalKit/MetalKit.h>
#endif

#include "graphics/descriptors.hpp"
#include "graphics/metal/rendertarget.hpp"
#include "graphics/metal/state.hpp"
#include "graphics/metal/staticbuffer.hpp"
#include "graphics/metal/dynamicbuffer.hpp"
#include "graphics/metal/texture.hpp"

namespace graphics::metal {
    class Context {
#ifdef __OBJC__
        dispatch_semaphore_t        _inFlightSemaphore;
        MTKView *                   _view;
        id<MTLDevice>               _device;
        id<MTLCommandQueue>         _commandQueue;
        id<MTLCommandBuffer>        _commandBuffer;
        id<MTLRenderCommandEncoder> _renderEncoder;
#else
        void *                      _inFlightSemaphore;
        void *                      _view;
        void *                      _device;
        void *                      _commandQueue;
        void *                      _commandBuffer;
        void *                      _renderEncoder;
#endif

        uint32_t                    _frame;

    public:
#ifdef __OBJC__
        Context(MTKView * view);
#endif
        ~Context();

        constexpr uint32_t getFrame()       const { return _frame; }
        constexpr uint32_t getNextFrame()   const { return (_frame + 1) % BUFFER_COUNT; }

        StaticBuffer * createStaticBuffer(uint32_t size, uint32_t count, const void * data);
        template <typename T>
        StaticBuffer * createStaticBuffer(uint32_t count, const T * data) {
            return createStaticBuffer(sizeof(T), count, data);
        }

        DynamicBuffer * createDynamicBuffer(uint32_t size, uint32_t capacity);
        template <typename T>
        DynamicBuffer * createDynamicBuffer(uint32_t capacity) {
            return createDynamicBuffer(sizeof(T), capacity);
        }

        Texture * createTexture(const TextureDesc & textureDesc);
        Texture * loadTexture(const char * textureName);

        RenderTarget * createRenderTarget(const RenderTargetDesc & targetDesc);
        void setRenderTarget(RenderTarget * target);
        void changeColorAttachment(RenderTarget * target, uint32_t id, Texture * texture);
        void changeDepthAttachment(RenderTarget * target, Texture * texture);

        State * createState(const StateDesc & desc);
        void setState(const State * state);

        void startFrame();
        void commitFrame();

        void setBuffer(uint32_t id, StaticBuffer * buffer, bool forFragmentShader = false);
        void setBuffer(uint32_t id, DynamicBuffer * buffer, bool forFragmentShader = false);
        void setTexture(uint32_t id, Texture * texture, bool forFragmentShader = true);

        void drawTriangles(uint32_t count);
        void drawTriangleStrip(uint32_t count);
        void drawIndexedTriangles(uint32_t count, StaticBuffer * indexBuffer);
        void drawIndexedTriangleStrip(uint32_t count, StaticBuffer * indexBuffer);

        void drawInstancedTriangles(uint32_t instanceCount, uint32_t vertexCount);
        void drawInstancedTriangleStrip(uint32_t instanceCount, uint32_t vertexCount);
        void drawInstancedIndexedTriangles(uint32_t instanceCount, uint32_t indexCount, StaticBuffer * indexBuffer);
        void drawInstancedIndexedTriangleStrip(uint32_t instanceCount, uint32_t indexCount, StaticBuffer * indexBuffer);
    };
}

#endif /* graphics_metal_context_hpp */
