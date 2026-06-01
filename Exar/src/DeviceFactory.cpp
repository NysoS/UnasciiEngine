#include "Exar/Exar.hpp"
#include "Exar/DeviceFactory.hpp"
#include "Exar/Device.hpp"

#ifdef _DEBUG
#include "Exar/Test/BufferAllocTest.hpp"
#include "Exar/Test/SwapchainPresentTest.hpp"
#endif // !_DEBUG


namespace Exar {
	extern "C" EXA_API void* RHI_Exar_CreateDevice(size_t pSize)
	{
		Exar::Device* lDevice = new Exar::Device();
		if (!lDevice->createMemory(pSize)) {
			delete lDevice;
			return nullptr;
		}

		return lDevice;
	}

	extern "C" EXA_API bool RHI_Exar_DestroyDevice(void* pHandle)
	{
		if (!pHandle) return false;

		Exar::Device* lDevice = static_cast<Exar::Device*>(pHandle);
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
	#endif
}