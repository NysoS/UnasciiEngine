set(SOURCES
    ExarAllocator.cpp
    ExarDeviceMemory.cpp
    Framebuffer.cpp
    Swapchain_del.cpp
)

register_sources(Exar ${SOURCES})

add_subdirectories(
    Windows
)