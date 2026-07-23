#include "Exar/Internal/Allocator.hpp"
#include "Exar/Internal/DeviceMemory.hpp"
#include "Exar/IRessource.hpp"
#include "Exar/Internal/AreaMemory.hpp"
#include "Exar/Internal/PageMemory.hpp"

Exar::Allocator::Allocator()
	: mVram(std::make_unique<DeviceMemory>(0))
	, mOffset(0)
	, mMemorySizeRemaining(0)
{
}

Exar::Allocator::~Allocator()
{
	if (!mVram) return;
	for (const auto& family : mVram->getAreaFamilies()) {
		if (!family) continue;
		VirtualFree(static_cast<void*>(family->getMemory()), 0, MEM_RELEASE);
	}
}

Exar::Result Exar::Allocator::createDeviceMemory(const DeviceMemoryCreateInfo& pDeviceMemoryCreateInfo)
{
	if (pDeviceMemoryCreateInfo.areaCount == 0) return Result::ERROR_INVALID_SIZE;
	if (!pDeviceMemoryCreateInfo.areaMemory) return Result::ERROR_ALLOCATOR_NULL_POINTER;

	LOG_ALLOC("createDeviceMemory","");

	for (size_t i = 0; i < pDeviceMemoryCreateInfo.areaCount; ++i) {
		const auto& lArea = pDeviceMemoryCreateInfo.areaMemory[i];

		LOG_ALLOC("Area", (u32)lArea.areaType);

		size_t lAreaSize = lArea.size;
		if (lAreaSize == 0) continue;

		void* lMemPtr = VirtualAlloc(NULL, lAreaSize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
		if (!lMemPtr) return Result::ERROR_MEMORY_ALLOCATION; //throw std::exception("Impossible to alloc size on device");

		Result lResult = mVram->createAreaMemory(lArea, (Memory)lMemPtr);
		if (lResult != Result::SUCCESS) return lResult;
	}

	return Result::SUCCESS;
}

void* Exar::Allocator::alloc(const AllocatorCreateInfo& pDesc, const MemoryRequirement& pRequirement) noexcept
{
	// Check area type and page index avaiblable
	if (pDesc.areaType == AreaMemoryType::NONE) return nullptr;
	if (pDesc.pageIndex == -1) return nullptr;

	// Get specific area
	auto* lArea = mVram->getAreaMemory(pDesc.areaType);
	if (!lArea) return nullptr;

	const PageMemory* lPage = lArea->getPage(pDesc.pageIndex);
	if (!lPage) return nullptr;

	size_t lMemorySizeRemaining = lPage->size;
	size_t lPageOffset = lPage->offset;

	// calcul offset + padding
	LOG_ALLOC("---------------", "");
	size_t lAlignValue = static_cast<size_t>(pRequirement.align);

	LOG_ALLOC("AllocDescTotalSize", pDesc.totalSize);
	LOG_ALLOC("AllocDescAlignMemory", (size_t)lAlignValue);

	LOG_ALLOC("---------------", "");
	LOG_ALLOC("Actual memory size remaining", lMemorySizeRemaining);

	LOG_ALLOC("Current offset", lPageOffset);
	size_t lAlignedOffset = (lPageOffset + (lAlignValue - 1)) & ~(lAlignValue - 1);
	LOG_ALLOC("Aligned offset", lAlignedOffset);

	size_t lSizeWithPadding = (lAlignedOffset - lPageOffset) + pRequirement.sizeInBytes;
	LOG_ALLOC("Data size", pRequirement.sizeInBytes);
	LOG_ALLOC("Data size with padding", lSizeWithPadding);
	if (lMemorySizeRemaining < lSizeWithPadding) return nullptr;

	// update data page
	Result lAllocPageResult = lArea->allocate(lSizeWithPadding, pDesc.pageIndex);
	if (lAllocPageResult != Result::SUCCESS) return nullptr;

	// alloc data to specific area
	u8* lVRam = reinterpret_cast<u8*>(lArea->getMemory());
	if (!lVRam) return nullptr;

	LOG_ALLOC("LVram", (uintptr_t)lVRam);
	u8* lTargetAddr = lVRam + lAlignedOffset;
	LOG_ALLOC("TargetOffset", (uintptr_t)lTargetAddr);
	void* lAddMem = VirtualAlloc(lTargetAddr, pRequirement.sizeInBytes, MEM_COMMIT, PAGE_READWRITE);
	if (!lAddMem) return nullptr;

	return lAddMem;
}

bool Exar::Allocator::dealloc(void* pRessource, const AllocatorCreateInfo& pDesc) noexcept
{
	if (pDesc.areaType == AreaMemoryType::NONE || pDesc.pageIndex == -1) return false;

	u8* lRessourceAddr = reinterpret_cast<u8*>(pRessource);
	if (!lRessourceAddr) return false;

	AreaMemory* lArea = mVram->getAreaMemory(pDesc.areaType);
	if (!lArea) return false;

	const PageMemory* lPage = lArea->getPage(pDesc.pageIndex);
	if (!lPage) return false;

	u8* lMemoryAddr = reinterpret_cast<u8*>(lArea->getMemory());
	if (!lMemoryAddr) return false;

	if (!VirtualFree(lMemoryAddr + lPage->startOffset, pDesc.totalSize, MEM_DECOMMIT)) return false;

	Result lDeallocPageResult = lArea->deallocate(pDesc.totalSize, pDesc.pageIndex);
	if (lDeallocPageResult != Result::SUCCESS) return false;

	pRessource = nullptr;

	return true;
}

size_t Exar::Allocator::getMemoryAreaSizeRemaining(const AreaMemoryType mAreaType) const noexcept
{
	if (!mVram) return 0;

	const auto* lArea = mVram->getAreaMemory(mAreaType);
	if (!lArea) return 0;

	return lArea->getSize();
}
