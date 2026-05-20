set (SOURCES 
    ExarBuffer.cpp
    ExarBufferView.cpp
    ExarDevice.cpp
    ExarDeviceFactory.cpp
)

add_subdirectories(
    Test
    Internal
)
register_sources(Exar ${SOURCES})
