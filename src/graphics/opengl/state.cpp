#include "graphics/opengl/state.hpp"

#include <cstdlib> // malloc, free

namespace graphics::opengl {
    // State::State() {

    // }

    State::~State() {
        // The `attributes` and `bufferLayouts` were allocated as a single buffer
        free((void *) _vertexDesc.attributes);
        glDeleteProgram(_shaderProgram);
    }
}
