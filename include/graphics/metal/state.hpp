#ifndef graphics_metal_state_hpp
#define graphics_metal_state_hpp

#ifdef __OBJC__
    #include <Metal/Metal.h>
#endif

#include "graphics/descriptors.hpp"

namespace graphics::metal {
    class Context;

    class State {
#ifdef __OBJC__
        id<MTLRenderPipelineState>  _renderPipelineState;
        id<MTLDepthStencilState>    _depthStencilState;
#else
        void *                      _renderPipelineState;
        void *                      _depthStencilState;
#endif

        CullMode                    _cullMode;
        Winding                     _winding;
        float                       _depthBias;
        float                       _depthSlopeScale;
        float                       _depthBiasClamp;

        friend class Context;

    public:
        ~State();
    };
}

#endif /* graphics_metal_state_hpp */
