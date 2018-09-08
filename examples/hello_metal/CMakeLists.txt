cmake_minimum_required(VERSION 3.10)

if(NOT XCODE)
    message(
    WARNING
        "Cannot create `hello_metal` example: The generator must be `Xcode` in order to compile metal shader files"
    )
else()
    project(hello_metal)
    set(CMAKE_CXX_STANDARD 17)

    add_executable(hello_metal)

    target_compile_options(hello_metal
    PUBLIC
        "-fobjc-arc"
    )

    set(SHADERS
        shader.metal
    )

    set_source_files_properties(
        ${SHADERS}
    PROPERTIES
        LANGUAGE METAL
    )

    target_sources(hello_metal
    PRIVATE
        main.mm
        AppDelegate.mm
        Renderer.mm
        ${SHADERS}
    )

    target_link_libraries(hello_metal
    PRIVATE
        graphics
        "-framework Cocoa"
        "-framework Metal"
        "-framework MetalKit"
    )
endif()
