#include "Exar/Exar.hpp"
#include "Exar/DeviceFactory.hpp"
#include "Exar/Device.hpp"

#ifdef _DEBUG
#include "Exar/Test/BufferAllocTest.hpp"
#include "Exar/Test/SwapchainPresentTest.hpp"
#include "Exar/Test/CommandPoolAllocationTest.hpp"
#include "Exar/Test/CommandBufferWithWorkerThread.hpp"
#endif // !_DEBUG


namespace Exar {
	extern "C" EXA_API void* RHI_Exar_CreateDevice(size_t pSize)
	{
		DeviceMemoryCreateInfo lInfo{};

		Exar::Device_* lDevice = new Exar::Device_();
		if (!lDevice->createDeviceMemory(lInfo)) {
			delete lDevice;
			return nullptr;
		}

		return lDevice;
	}

	extern "C" EXA_API bool RHI_Exar_DestroyDevice(void* pHandle)
	{
		if (!pHandle) return false;

		Exar::Device_* lDevice = static_cast<Exar::Device_*>(pHandle);
		if (!lDevice) return false;

		delete lDevice;
		return true;
	}

	#ifdef _DEBUG
	extern "C" EXA_API void RHI_Verif_Allocation()
	{
		BufferAllocTest lTest;
	}

	extern "C" EXA_API void RHI_Swapchain_Present_Test()
	{
		SwapchainPresentTest lTest;
	}

	extern "C" EXA_API void RHI_CommandPool_Allocation_Test()
	{
		CommandPoolAllocationTest lTest;
	}

	extern "C" EXA_API void RHI_CommandBuffer_With_WorkerThreaad_Test()
	{
		CommandBufferWithWorkerThread lTest;
	}
	#endif
}