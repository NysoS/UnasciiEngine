cmake_minimum_required(VERSION 3.8)

project(UnasciiEngine VERSION 0.1.0 LANGUAGES C CXX)

add_subdirectory(Engine)
add_subdirectory(Unascii)

install(
    TARGETS
        Engine
        Unascii
    RUNTIME DESTINATION bin
)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Unascii)