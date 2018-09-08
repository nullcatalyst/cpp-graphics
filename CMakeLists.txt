cmake_minimum_required(VERSION 3.10)

project(graphics)
set(CMAKE_CXX_STANDARD 17)

# OPTIONS

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

option(BACKEND_METAL    "Build the Metal backend"   ON)
option(BACKEND_OPENGL   "Build the OpenGL backend"  OFF)

# TARGET

add_library(graphics STATIC)

target_include_directories(graphics
PUBLIC
    include
PRIVATE
    src
)

# BACKENDS

if(BACKEND_METAL)
    target_compile_options(graphics
    PUBLIC
        "-fobjc-arc"
    )

    target_sources(graphics
    PRIVATE
        src/graphics/metal/context.mm
        # src/graphics/metal/convert.mm
        src/graphics/metal/rendertarget.mm
        src/graphics/metal/state.mm
        src/graphics/metal/staticbuffer.mm
        src/graphics/metal/dynamicbuffer.mm
        src/graphics/metal/texture.mm
    )
endif()

if(BACKEND_OPENGL)
    target_sources(graphics
    PRIVATE
        # src/graphics/opengl/*
    )
endif()
