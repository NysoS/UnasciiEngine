#include "Engine/Runtime/RHI/RHIDef.hpp"
#include "Engine/Runtime/RHI/IDevice.hpp"
#include "Engine/Runtime/RHI/IDeviceFactory.hpp"
#include "Engine/Runtime/RHI/Manager.hpp"

namespace UnasciiEngine::RHI
{
	#pragma comment(lib, "Exar.lib")
	extern "C" __declspec(dllimport) void* RHI_Exar_CreateDevice(size_t pSize);
	extern "C" __declspec(dllimport) bool RHI_Exar_DestroyDevice(void* pHandle);

#ifdef _DEBUG
#include "Engine/Runtime/RHI/Test/IRHITest.hpp"

	extern "C" __declspec(dllimport) void RHI_Verif_Allocation();
	extern "C" __declspec(dllimport) void RHI_Swapchain_Present_Test();
	extern "C" __declspec(dllimport) void RHI_CommandPool_Allocation_Test();

	class ExarTest : public UnasciiEngine::RHI::Test::IRHIBufferAllocationTest {
	public:
		ExarTest() = default;
		virtual ~ExarTest() = default;

		virtual void execute() override {
			RHI_Verif_Allocation();
		}
	};

	class ExarSwapchainTest : public UnasciiEngine::RHI::Test::IRHISwapchainTest {
	public: 
		ExarSwapchainTest() = default;
		virtual ~ExarSwapchainTest() = default;

		virtual void execute() override {
			RHI_Swapchain_Present_Test();
		}
	};

	class ExarCommandPoolTest : public UnasciiEngine::RHI::Test::IRHICommandPoolTest {
	public:
		ExarCommandPoolTest() = default;
		virtual ~ExarCommandPoolTest() = default;

		virtual void execute() override {
			RHI_CommandPool_Allocation_Test();
		}
	};

#endif // DEBUG

	class ExarDeviceBridge;

	class ExarDeviceFactory : public IDeviceFactory
	{
	public:
		ExarDeviceFactory() = default;
		virtual ~ExarDeviceFactory() = default;

		IDevice* CreateDevice(size_t pMemorySize = (1024 * 1024 * 1024)) override;

		bool DestroyDevice(IDevice* pDevice) override;
	};

	namespace Factory
	{
#ifdef _DEBUG
		std::unique_ptr<Test::IRHIBufferAllocationTest> createBufferAllocationTest()
		{
			return std::make_unique<ExarTest>();
		}

		std::unique_ptr<Test::IRHISwapchainTest> createSwapchainPresentTest()
		{
			return std::make_unique<ExarSwapchainTest>();
		}

		std::unique_ptr<Test::IRHICommandPoolTest> createCommandPoolAllocationTest()
		{
			return std::make_unique<ExarCommandPoolTest>();
		}
#endif // _DEBUG

		std::unique_ptr<IDeviceFactory> createDeviceFactory()
		{
			return std::make_unique<ExarDeviceFactory>();
		}
	}

	class ExarDeviceBridge : public IDevice
	{
	public:
		friend class ExarDeviceFactory;

		ExarDeviceBridge(RHIDeviceHandle phandle)
			: mHandle(phandle)
		{
		}
		
		virtual ~ExarDeviceBridge()
		{
			mHandle = nullptr;
		}

	private:
		RHIDeviceHandle mHandle;
	};

	IDevice* ExarDeviceFactory::CreateDevice(size_t pMemorySize)
	{
		void* lDeviceHandle = RHI_Exar_CreateDevice(pMemorySize);

		return lDeviceHandle ? new ExarDeviceBridge(static_cast<RHIDeviceHandle>(lDeviceHandle)) : nullptr;
	}
	
	bool ExarDeviceFactory::DestroyDevice(IDevice* pDevice)
	{
		if (!pDevice) return false;

		ExarDeviceBridge* lDevice = reinterpret_cast<ExarDeviceBridge*>(pDevice);

		if (!RHI_Exar_DestroyDevice(lDevice->mHandle)) return false;

		delete lDevice;
		lDevice = nullptr;

		return true;
	}
}
