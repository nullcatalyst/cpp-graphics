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
endif()
