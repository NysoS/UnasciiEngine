#ifndef EXAR_ALLOCATOR_HPP
#define EXAR_ALLOCATOR_HPP

#include "Engine/uaepch.h"
#include "Engine/Runtime/RHI/Exar/ExarAllocatorDesc.hpp"
#include "Engine/Runtime/RHI/RHIDef.hpp"

namespace UnasciiEngine::RHI::EXAR
{
	class ExarAllocator {
	public:

		explicit ExarAllocator(const ExarAllocatorAreaDesc& pDesc);
		~ExarAllocator();

		ExarAllocator(const ExarAllocator&) = delete;
		ExarAllocator& operator=(const ExarAllocator&) = delete;

		MemHandle alloc(const size_t pSize, const void* pData);
	};
}

#endif // !EXAR_ALLOCATOR_HPP
