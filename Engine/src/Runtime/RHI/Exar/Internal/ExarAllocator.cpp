#include "Engine/Runtime/RHI/Exar/Internal/ExarAllocator.hpp"
#include "Engine/Runtime/RHI/Exar/Internal/ExarDeviceMemory.hpp"
#include "Engine/Runtime/RHI/Exar/Internal/MemoryArena.hpp"
#include "Engine/Runtime/RHI/Exar/IExarRessource.hpp"

UnasciiEngine::RHI::EXAR::ExarAllocator::ExarAllocator(ExarDeviceMemory* pDeviceMemory)
	: mVram(pDeviceMemory)
	, mOffset(0)
	, mMemorySizeRemaining(0)
{
	if (pDeviceMemory)
	{
		mMemorySizeRemaining = pDeviceMemory->getMemorySize();
	}
}

UnasciiEngine::RHI::EXAR::ExarAllocator::~ExarAllocator()
{
}

UnasciiEngine::RHI::MemHandle UnasciiEngine::RHI::EXAR::ExarAllocator::alloc(const ExarAllocatorDesc& pDesc, const ExarMemoryRequirement& pRequirement)
{
	size_t lAlignedOffset = (mOffset + (pRequirement.align - 1)) & ~(pRequirement.align - 1);

	size_t lSizeWithPadding = (lAlignedOffset - mOffset) + pRequirement.sizeInBytes;
	if (mMemorySizeRemaining < lSizeWithPadding) return nullptr;

	u8* lVRam = reinterpret_cast<u8*>(mVram->getMemoryHandle());
	if (!lVRam) return nullptr;
	
	u8* lTargetAddr = lVRam + lAlignedOffset;
	void* lAddMem = VirtualAlloc(lTargetAddr, pRequirement.sizeInBytes, MEM_COMMIT, PAGE_READWRITE);
	if (!lAddMem) return nullptr;

	mOffset += lSizeWithPadding;
	mMemorySizeRemaining -= lSizeWithPadding;

	// cast to opac pointer (imcomplete struct) for bind on buffer
	return static_cast<MemHandle>(lAddMem);
}

bool UnasciiEngine::RHI::EXAR::ExarAllocator::dealloc(size_t pSize, MemHandle& pRessource) noexcept
{
	if (pSize <= 0) return false;

	u8* lRessourceAddr = reinterpret_cast<u8*>(pRessource);
	if (!lRessourceAddr) return false;

	u8* lVaram = reinterpret_cast<u8*>(mVram->getMemoryHandle());
	if (!lVaram) return false;

	//if (!VirtualFree(lRessourceAddr, pSize, MEM_DECOMMIT)) return false;

	if (lRessourceAddr + pSize == lVaram + mOffset)
	{
		mOffset = static_cast<size_t>(lRessourceAddr - lVaram);
		mMemorySizeRemaining = mVram->getMemorySize() - mOffset;
	}
	pRessource = nullptr;

	return true;
}

//UnasciiEngine::RHI::MemHandle UnasciiEngine::RHI::EXAR::ExarAllocator::alloc(std::span<const u8> pRessource)
//{
//	//MemoryArena::instanceMemory().alloc(pRessource.size());
//
//	return nullptr;
//}
