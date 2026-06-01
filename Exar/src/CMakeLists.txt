set (SOURCES 
    Buffer.cpp
    BufferView.cpp
    Device.cpp
    DeviceFactory.cpp
)

add_subdirectories(
    Test
    Internal
)
register_sources(Exar ${SOURCES})
