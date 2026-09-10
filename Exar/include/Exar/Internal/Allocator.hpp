#ifndef EXAR_ALLOCATOR_HPP
#define EXAR_ALLOCATOR_HPP

#include "Exar/MinimalCore.hpp"
#include "Exar/Descriptor.hpp"

#ifdef _DEBUG
#include <iostream>
#define LOG_ALLOC(type, value) \
	std::cout << type << " [ " << value << " ] " << std::endl
#else 
#define LOG_ALLOC(type, value)
#endif

namespace Exar
{
	class DeviceMemory;
	class IRessource;
	class AreaMemory;

	class Allocator 
	{
	public:
		explicit Allocator();
		~Allocator();

		Allocator(const Allocator&) = delete;
		Allocator& operator=(const Allocator&) = delete;

		Result createDeviceMemory(const DeviceMemoryCreateInfo& pDeviceMemoryCreateInfo);

		void* alloc(const AllocatorCreateInfo& pDesc, const MemoryRequirement& pRequirement) noexcept;
		bool dealloc(void* pRessource, const AllocatorCreateInfo& pDesc) noexcept;

		size_t getMemoryAreaSizeRemaining(const AreaMemoryType mAreaType) const noexcept;
		void* findRessource(const FindRessourceInfo& pFindInfo) const noexcept;

	private:

		std::unique_ptr<DeviceMemory> mVram; // Device has a owner ptr
		size_t mOffset;
		size_t mMemorySizeRemaining;
	};
}

#endif // !EXAR_ALLOCATOR_HPP
