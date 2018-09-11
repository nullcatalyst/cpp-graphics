#include "graphics/opengl/rendertarget.hpp"

namespace graphics::opengl {
    RenderTarget::~RenderTarget() {
        glDeleteFramebuffers(1, &_fbo);
    }
}
