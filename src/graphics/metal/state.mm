#include "graphics/metal/state.hpp"

namespace graphics::metal {
    State::~State() {
        _renderPipelineState    = nil;
        _depthStencilState      = nil;
    }
}
