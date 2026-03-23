#ifndef EXAR_DEVICE_MEMORY_HPP
#define EXAR_DEVICE_MEMORY_HPP

#include "Engine/Core/Types.hpp"
#include "Engine/Runtime/RHI/RHIDef.hpp"

namespace UnasciiEngine::RHI::EXAR
{
	class ExarDeviceMemory final
	{
	public:
		explicit ExarDeviceMemory(size_t pSize);
		~ExarDeviceMemory();

		ExarDeviceMemory(const ExarDeviceMemory&) = delete;
		ExarDeviceMemory& operator=(const ExarDeviceMemory&) = delete;

		MemHandle getMemoryHandle();
		size_t getMemorySize() const;

	private:
		MemHandle mHandle;
		size_t mSize;
	};
}

#endif // !EXAR_DEVICE_MEMORY_HPP
