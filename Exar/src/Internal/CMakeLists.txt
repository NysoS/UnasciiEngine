set(SOURCES
    Allocator.cpp
    DeviceMemory.cpp
    AreaMemory.cpp
    Framebuffer.cpp
    Swapchain_del.cpp
)

register_sources(Exar ${SOURCES})

add_subdirectories(
    Windows
)