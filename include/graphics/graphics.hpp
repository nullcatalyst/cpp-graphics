#ifndef graphics_graphics_hpp
#define graphics_graphics_hpp

#include "util/os.hpp"

#if !(GRAPHICS_USE_METAL || GRAPHICS_USE_OPENGL)
    // Try to determine best backend given the target platform
    #if OS(mac) || OS(ios)
        #define GRAPHICS_USE_METAL 1
    #else
        #define GRAPHICS_USE_OPENGL 1
    #endif
#endif

#if GRAPHICS_USE_METAL
    #include "graphics/metal/context.hpp"
    namespace graphics {
        using namespace metal;
    }
#endif

#if GRAPHICS_USE_OPENGL
    #include "graphics/opengl/graphics.hpp"
    namespace graphics {
        using namespace opengl;
    }
#endif

#endif /* graphics_graphics_hpp */
