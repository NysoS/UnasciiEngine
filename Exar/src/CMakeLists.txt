set (SOURCES 
    Buffer.cpp
    BufferView.cpp
    Device.cpp
    DeviceFactory.cpp
    API.cpp
)

add_subdirectories(
    Test
    Internal
)
register_sources(Exar ${SOURCES})
