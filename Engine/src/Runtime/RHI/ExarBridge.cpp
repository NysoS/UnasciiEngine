#include "Engine/Runtime/RHI/RHIDef.hpp"
#include "Engine/Runtime/RHI/IDevice.hpp"
#include "Engine/Runtime/RHI/DeviceFactory.hpp"

namespace UnasciiEngine::RHI
{
	#pragma comment(lib, "Exar.lib")
	extern "C" __declspec(dllimport) void* RHI_Exar_CreateDevice(size_t pSize);
	extern "C" __declspec(dllimport) bool RHI_Exar_DestroyDevice(void* pHandle);

	class ExarDeviceBridge;

	class ExarDeviceFactory : public IDeviceFactory
	{
	public:
		ExarDeviceFactory() = default;
		virtual ~ExarDeviceFactory() = default;

		IDevice* CreateDevice(size_t pMemorySize = (1024 * 1024 * 1024)) override;

		bool DestroyDevice(IDevice* pDevice) override;
	};

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
