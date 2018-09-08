#include "graphics/metal/rendertarget.hpp"

namespace graphics::metal {
    RenderTarget::RenderTarget(MTLRenderPassDescriptor * renderPassDescriptor) : _renderPassDescriptor(renderPassDescriptor) {}

    RenderTarget::~RenderTarget() {
        _renderPassDescriptor = nil;
    }
}
