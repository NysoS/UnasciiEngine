set (SOURCES 
    BufferAllocTest.cpp
    SwapchainPresentTest.cpp
    CommandPoolAllocationTest.cpp
    CommandBufferWithWorkerThread.cpp
)

register_sources(Exar ${SOURCES})


# target_sources(Exar PRIVATE SRC_DIR)