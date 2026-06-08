#ifndef EXAR_DEVICE_MEMORY_HPP
#define EXAR_DEVICE_MEMORY_HPP

#include "Exar/MinimalCore.hpp"

namespace Exar
{
	class DeviceMemory final
	{
	public:
		explicit DeviceMemory(size_t pSize);
		~DeviceMemory();

		DeviceMemory(const DeviceMemory&) = delete;
		DeviceMemory& operator=(const DeviceMemory&) = delete;

		Memory getMemory();
		size_t getMemorySize() const;

	private:
		Memory mHandle;
		size_t mSize;
	};
}

#endif // !EXAR_DEVICE_MEMORY_HPP
