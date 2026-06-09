#include "Exar/Internal/Allocator.hpp"
#include "Exar/Internal/DeviceMemory.hpp"
#include "Exar/IRessource.hpp"

Exar::Allocator::Allocator(const DeviceMemoryCreateInfo& pDeviceMemoryCreateInfo)
	: mVram(std::make_unique<DeviceMemory>(0))
	, mOffset(0)
	, mMemorySizeRemaining(0)
{
	// reserve pour chaque type de memory ajouter ds le deviceMemoryCreateInfo

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
	// recup le type d'area ou return si null

	// recup la page a gerer

	// calcul d'offset + padding

	// alloue la bonne area

	// mettre a jour les info size des pages et area

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

	u8* lVRam = reinterpret_cast<u8*>(mVram->getMemory());
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

bool Exar::Allocator::dealloc(size_t pSize, Memory& pRessource) noexcept
{
	if (pSize <= 0) return false;

	u8* lRessourceAddr = reinterpret_cast<u8*>(pRessource);
	if (!lRessourceAddr) return false;

	u8* lVaram = reinterpret_cast<u8*>(mVram->getMemory());
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