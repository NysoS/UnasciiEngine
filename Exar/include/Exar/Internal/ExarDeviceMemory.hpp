#ifndef EXAR_DEVICE_MEMORY_HPP
#define EXAR_DEVICE_MEMORY_HPP

#include "Exar/Types.hpp"
#include "Exar/Def.hpp"

namespace Exar
{
	class ExarDeviceMemory final
	{
	public:
		explicit ExarDeviceMemory(size_t pSize);
		~ExarDeviceMemory();

		ExarDeviceMemory(const ExarDeviceMemory&) = delete;
		ExarDeviceMemory& operator=(const ExarDeviceMemory&) = delete;

		MemoryHandle getMemoryHandle();
		size_t getMemorySize() const;

	private:
		MemoryHandle mHandle;
		size_t mSize;
	};
}

#endif // !EXAR_DEVICE_MEMORY_HPP
