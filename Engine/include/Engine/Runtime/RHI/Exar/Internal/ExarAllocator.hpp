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
		explicit ExarAllocator(ExarDeviceMemory* pDeviceMemory);
		~ExarAllocator();

		ExarAllocator(const ExarAllocator&) = delete;
		ExarAllocator& operator=(const ExarAllocator&) = delete;

		MemHandle alloc(const ExarAllocatorDesc& pDesc, const ExarMemoryRequirement& pRequirement) noexcept;
		bool dealloc(size_t pSize, MemHandle& pRessource) noexcept;

	private:
		ExarDeviceMemory* mVram; // Device has a owner ptr
		size_t mOffset;
		size_t mMemorySizeRemaining;
	};
}

#endif // !EXAR_ALLOCATOR_HPP
