#ifndef DEVICE_FACTORY_HPP
#define DEVICE_FACTORY_HPP

#include "Engine/Runtime/RHI/RHIDef.hpp"
#include "Engine/Runtime/RHI/IDevice.hpp"

#ifdef _DEBUG
#include "Engine/Runtime/RHI/Test/IRHITest.hpp"
#endif // _DEBUG


namespace UnasciiEngine::RHI
{
	class IDeviceFactory
	{
	public:
		virtual ~IDeviceFactory() = default;
		virtual IDevice* CreateDevice(size_t pMemorySize = (1024 * 1024 * 1024)) = 0;
		virtual bool DestroyDevice(IDevice* pDevice) = 0;
	};

	namespace Factory
	{
#ifdef _DEBUG
		std::unique_ptr<Test::IRHIBufferAllocationTest> createBufferAllocationTest();
		std::unique_ptr<Test::IRHISwapchainTest> createSwapchainPresentTest();
		std::unique_ptr<Test::IRHICommandPoolTest> createCommandPoolAllocationTest();
		std::unique_ptr<Test::IRHICommandBufferWithWorkerThread> createCommandBufferWithWorkerThread();
#endif // _DEBUG

		std::unique_ptr<IDeviceFactory> createDeviceFactory();
	}
}

#endif // !DEVICE_FACTORY_HPP
