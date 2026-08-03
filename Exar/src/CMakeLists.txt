set (SOURCES 
    Buffer.cpp
    BufferView.cpp
    Device.cpp
    DeviceFactory.cpp
    API.cpp
    ImageView.cpp
)

add_subdirectories(
    Test
    Internal
)
register_sources(Exar ${SOURCES})
