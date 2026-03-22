#include "Engine/Runtime/RHI/Exar/Internal/ExarAllocator.hpp"
#include "Engine/Runtime/RHI/Exar/Internal/MemoryArena.hpp"

UnasciiEngine::RHI::EXAR::ExarAllocator::ExarAllocator(const ExarAllocatorAreaDesc& pDesc)
{
}

UnasciiEngine::RHI::EXAR::ExarAllocator::~ExarAllocator()
{
}

UnasciiEngine::RHI::MemHandle UnasciiEngine::RHI::EXAR::ExarAllocator::alloc(std::span<const u8> pRessource)
{
	//MemoryArena::instanceMemory().alloc(pRessource.size());

	return nullptr;
}
