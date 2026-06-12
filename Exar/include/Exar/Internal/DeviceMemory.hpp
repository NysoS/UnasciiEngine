#ifndef EXAR_DEVICE_MEMORY_HPP
#define EXAR_DEVICE_MEMORY_HPP

#include "Exar/MinimalCore.hpp"
#include "Exar/Descriptor.hpp"

namespace Exar
{
	class AreaMemory;

	class DeviceMemory final
	{
	public:
		explicit DeviceMemory(size_t pSize);
		~DeviceMemory();

		Result createAreaMemory(const AreaMemoryCreateInfo& pAreaCreateInfo, Memory lVram);

		DeviceMemory(const DeviceMemory&) = delete;
		DeviceMemory& operator=(const DeviceMemory&) = delete;

		Memory getMemory();
		size_t getMemorySize() const;

	private:
		std::unique_ptr<AreaMemory> mSwapchainArea;

		Memory mHandle;
		size_t mSize;
	};
}

#endif // !EXAR_DEVICE_MEMORY_HPP
