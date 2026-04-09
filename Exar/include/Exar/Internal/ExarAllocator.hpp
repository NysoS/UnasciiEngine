#ifndef EXAR_ALLOCATOR_HPP
#define EXAR_ALLOCATOR_HPP

#include "Exar/exarpch.h"
#include "Exar/Types.hpp"
#include "Exar/ExarAllocatorDesc.hpp"
#include "Exar/ExarMemoryRequirements.hpp"
#include "Exar/Def.hpp"

#include <span>

#ifdef _DEBUG
#include <iostream>
#define LOG_ALLOC(type, value) \
	std::cout << type << " [ " << value << " ] " << std::endl
#else 
#define LOG_ALLOC(type, value)
#endif

namespace Exar
{
	class ExarDeviceMemory;
	class IExarRessource;

	class ExarAllocator 
	{
	public:
		explicit ExarAllocator(size_t pMemorySize);
		~ExarAllocator();

		ExarAllocator(const ExarAllocator&) = delete;
		ExarAllocator& operator=(const ExarAllocator&) = delete;

		MemoryHandle alloc(const ExarAllocatorDesc& pDesc, const ExarMemoryRequirement& pRequirement) noexcept;
		bool dealloc(size_t pSize, MemoryHandle& pRessource) noexcept;

		inline size_t getMemorySizeRemaining() const noexcept {
			return mMemorySizeRemaining;
		}

	private:
		std::unique_ptr<ExarDeviceMemory> mVram; // Device has a owner ptr
		size_t mOffset;
		size_t mMemorySizeRemaining;
	};
}

#endif // !EXAR_ALLOCATOR_HPP
