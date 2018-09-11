#ifndef graphics_opengl_context_hpp
#define graphics_opengl_context_hpp

#include "graphics/descriptors.hpp"
#include "graphics/opengl/gl.hpp"
#include "graphics/opengl/rendertarget.hpp"
#include "graphics/opengl/state.hpp"
#include "graphics/opengl/staticbuffer.hpp"
#include "graphics/opengl/dynamicbuffer.hpp"
#include "graphics/opengl/texture.hpp"

namespace graphics::opengl {
    using LoadShaderFn = const char * (*)(const char * fileName);

    class Context {
        uint32_t                    _frame;
        const State *               _activeState;
        LoadShaderFn                _loadShader;

    public:
        Context();
        Context(LoadShaderFn loadShader);
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
        void setRenderTarget(const RenderTarget * target);
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

#endif /* graphics_opengl_context_hpp */
