#ifndef graphics_metal_texture_hpp
#define graphics_metal_texture_hpp

#ifdef __OBJC__
    #include <Metal/Metal.h>
#endif

namespace graphics::metal {
    class Context;
    class RenderTarget;

    class Texture {
#ifdef __OBJC__
        id<MTLTexture>  _texture;
#else
        void *          _texture;
#endif

        friend class Context;
        friend class RenderTarget;

    public:
        ~Texture();
    };
}

#endif /* graphics_metal_texture_hpp */
