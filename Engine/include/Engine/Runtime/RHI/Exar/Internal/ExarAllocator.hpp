#ifndef EXAR_ALLOCATOR_HPP
#define EXAR_ALLOCATOR_HPP

#include "Engine/uaepch.h"
#include "Engine/Core/Types.hpp"
#include "Engine/Runtime/RHI/Exar/ExarAllocatorDesc.hpp"
#include "Engine/Runtime/RHI/Exar/ExarMemoryRequirements.hpp"
#include "Engine/Runtime/RHI/RHIDef.hpp"

#include <span>

namespace UnasciiEngine::RHI::EXAR
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

		MemHandle alloc(const ExarAllocatorDesc& pDesc, const ExarMemoryRequirement& pRequirement) noexcept;
		bool dealloc(size_t pSize, MemHandle& pRessource) noexcept;

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
