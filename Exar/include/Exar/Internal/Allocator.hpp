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

	class Allocator 
	{
	public:
		explicit Allocator();
		~Allocator();

		Allocator(const Allocator&) = delete;
		Allocator& operator=(const Allocator&) = delete;

		Result createDeviceMemory(const DeviceMemoryCreateInfo& pDeviceMemoryCreateInfo);

		void* alloc(const AllocatorCreateInfo& pDesc, const MemoryRequirement& pRequirement) noexcept;
		bool dealloc(size_t pSize, Memory& pRessource) noexcept;

		inline size_t getMemorySizeRemaining() const noexcept {
			return mMemorySizeRemaining;
		}

	private:

		std::unique_ptr<DeviceMemory> mVram; // Device has a owner ptr
		size_t mOffset;
		size_t mMemorySizeRemaining;
	};
}

#endif // !EXAR_ALLOCATOR_HPP
