set (HEADERS 
    API.hpp
    BufferTypes.hpp
    BufferView.hpp
    Core.hpp
    Descriptor.hpp
    Device.hpp
    DeviceFactory.hpp
    Exar.hpp
    exarpch.h
    Extent.hpp
    FramebufferInfo.hpp
    IBuffer.hpp
    IDevice.hpp
    ImageView.hpp
    IObject.hpp
    IRessource.hpp
    ISwapchain.hpp
    MinimalCore.hpp
    SwapchainDesc.hpp
    Types.hpp
    Fence.hpp
)

add_subdirectories(
    Test
    Internal
)
register_sources(Exar ${HEADERS})
