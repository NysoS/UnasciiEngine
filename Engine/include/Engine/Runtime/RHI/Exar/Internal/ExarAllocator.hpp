#ifndef EXAR_ALLOCATOR_HPP
#define EXAR_ALLOCATOR_HPP

#include "Engine/uaepch.h"
#include "Engine/Core/Types.hpp"
#include "Engine/Runtime/RHI/Exar/ExarAllocatorDesc.hpp"
#include "Engine/Runtime/RHI/RHIDef.hpp"

#include <span>

namespace UnasciiEngine::RHI::EXAR
{
	class ExarAllocator 
	{
	public:

		explicit ExarAllocator(const ExarAllocatorAreaDesc& pDesc);
		~ExarAllocator();

		ExarAllocator(const ExarAllocator&) = delete;
		ExarAllocator& operator=(const ExarAllocator&) = delete;

		MemHandle alloc(std::span<const u8> pRessource);
	};
}

#endif // !EXAR_ALLOCATOR_HPP
