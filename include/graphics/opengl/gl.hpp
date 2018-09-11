#ifndef graphics_opengl_gl_hpp
#define graphics_opengl_gl_hpp

#include "util/os.hpp"

#if OS(mac)
    #define GL_SILENCE_DEPRECATION 1
    #include <OpenGL/gl3.h>
#else
    #include "graphics/opengl/glad.h"
#endif

#endif /* graphics_opengl_gl_hpp */
