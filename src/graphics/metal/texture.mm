#include "graphics/metal/texture.hpp"

namespace graphics::metal {
    Texture::~Texture() {
        _texture = nil;
    }
}
