set (HEADERS 
    IDevice.hpp
    IDeviceFactory.hpp
    Manager.hpp
    RHIDef.hpp
)

add_subdirectories(
    Test
)

register_sources(Engine ${HEADERS})