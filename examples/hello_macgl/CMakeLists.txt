cmake_minimum_required(VERSION 3.10)

if(NOT APPLE)
    message(
    WARNING
        "Cannot create `hello_macgl` example"
    )
else()
    project(hello_macgl)
    set(CMAKE_CXX_STANDARD 17)

    add_executable(hello_macgl)

    set(SHADERS
        ${PROJECT_SOURCE_DIR}/default_vertex.glsl
        ${PROJECT_SOURCE_DIR}/default_fragment.glsl
    )

    set_target_properties(hello_macgl
    PROPERTIES
        XCODE_ATTRIBUTE_CLANG_ENABLE_OBJC_ARC YES
    )

    target_compile_options(hello_macgl
    PUBLIC
        "-fobjc-arc"
    )

    target_sources(hello_macgl
    PRIVATE
        main.mm
        AppDelegate.mm
        GLView.mm
    )

    target_link_libraries(hello_macgl
    PRIVATE
        graphics
        "-framework Cocoa"
        "-framework CoreVideo"
        "-framework OpenGL"
    )

    add_custom_command(
    TARGET
        hello_macgl
    POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${SHADERS} $<TARGET_FILE_DIR:hello_macgl>
    )
endif()
