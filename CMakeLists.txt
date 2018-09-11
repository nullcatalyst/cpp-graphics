cmake_minimum_required(VERSION 3.10)

project(graphics)
set(CMAKE_CXX_STANDARD 17)

# OPTIONS

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

option(BACKEND_METAL    "Build the Metal backend"       ON)
option(BACKEND_OPENGL   "Build the OpenGL backend"      OFF)

option(BUILD_EXAMPLES   "Build the example projects"    ON)

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
    set_target_properties(graphics
    PROPERTIES
        XCODE_ATTRIBUTE_CLANG_ENABLE_OBJC_ARC YES
    )

    target_compile_options(graphics
    PUBLIC
        "-fobjc-arc"
    )

    target_compile_definitions(graphics
    PUBLIC
        GRAPHICS_USE_METAL=1
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
    target_compile_definitions(graphics
    PUBLIC
        GRAPHICS_USE_OPENGL=1
    )

    target_sources(graphics
    PRIVATE
        # src/graphics/opengl/*
    )
endif()

# EXAMPLES

if(BUILD_EXAMPLES)
    if(BACKEND_METAL)
        add_subdirectory(examples/hello_metal)
    endif()

    if(BACKEND_OPENGL AND APPLE)
        add_subdirectory(examples/hello_macgl)
    endif()
endif()
