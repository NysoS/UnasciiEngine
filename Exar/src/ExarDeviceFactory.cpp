#include "Exar/Exar.hpp"
#include "Exar/ExarDeviceFactory.hpp"
#include "Exar/ExarDevice.hpp"

extern "C" EXA_API void* RHI_Exar_CreateDevice(size_t pSize)
{
	Exar::ExarDevice* lDevice = new Exar::ExarDevice();
	if (!lDevice->createMemory(pSize)) {
		delete lDevice;
		return nullptr;
	}

	return lDevice;
}

extern "C" EXA_API bool RHI_Exar_DestroyDevice(void* pHandle)
{
	if (!pHandle) return false;

	Exar::ExarDevice* lDevice = static_cast<Exar::ExarDevice*>(pHandle);
	if (!lDevice) return false;

	delete lDevice;
	return true;
}