#include "Exar/Internal/Allocator.hpp"
#include "Exar/Internal/DeviceMemory.hpp"
#include "Exar/IRessource.hpp"

Exar::Allocator::Allocator(size_t pMemorySize)
	: mVram(std::make_unique<DeviceMemory>(pMemorySize))
	, mOffset(0)
	, mMemorySizeRemaining(0)
{
	if (mVram)
	{
		mMemorySizeRemaining = mVram->getMemorySize();
	}
}

Exar::Allocator::~Allocator()
{
}

void* Exar::Allocator::alloc(const AllocatorCreateInfo& pDesc, const MemoryRequirement& pRequirement) noexcept
{
	// Telemetri allocator //
	// Desc
	LOG_ALLOC("---------------", "");
	size_t lAlignValue = static_cast<size_t>(pRequirement.align);

	LOG_ALLOC("AllocDescTotalSize", pDesc.totalSize);
	LOG_ALLOC("AllocDescAlignMemory", (size_t)lAlignValue);
	LOG_ALLOC("AllocDescAllocLocation", (u32)pDesc.allocLocation);

	LOG_ALLOC("---------------", "");
	LOG_ALLOC("Actual memory size remaining", mMemorySizeRemaining);

	LOG_ALLOC("Current offset", mOffset);
	size_t lAlignedOffset = (mOffset + (lAlignValue - 1)) & ~(lAlignValue - 1);
	LOG_ALLOC("Aligned offset", lAlignedOffset);

	size_t lSizeWithPadding = (lAlignedOffset - mOffset) + pRequirement.sizeInBytes;
	LOG_ALLOC("Data size", pRequirement.sizeInBytes);
	LOG_ALLOC("Data size with padding", lSizeWithPadding);
	if (mMemorySizeRemaining < lSizeWithPadding) return nullptr;

	u8* lVRam = reinterpret_cast<u8*>(mVram->getMemoryHandle());
	if (!lVRam) return nullptr;
	
	LOG_ALLOC("LVram", (uintptr_t)lVRam);
	u8* lTargetAddr = lVRam + lAlignedOffset;
	LOG_ALLOC("TargetOffset", (uintptr_t)lTargetAddr);
	void* lAddMem = VirtualAlloc(lTargetAddr, pRequirement.sizeInBytes, MEM_COMMIT, PAGE_READWRITE);
	if (!lAddMem) return nullptr;

	mOffset += lSizeWithPadding;
	mMemorySizeRemaining -= lSizeWithPadding;

	LOG_ALLOC("New offset", mOffset);
	LOG_ALLOC("Memory Size Remaining after allocation", mMemorySizeRemaining);

	// cast to opac pointer (imcomplete struct) for bind on buffer
	return lAddMem;
}

bool Exar::Allocator::dealloc(size_t pSize, MemoryHandle& pRessource) noexcept
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